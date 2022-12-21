#!/usr/bin/env python3

"""This command line utility helps automate some builds activities.
The available commands are explained below for quick reference.

# Sets and/or shows the current version of the project under development
./buildtool version

# Runs Bazel to outputs the necessary binaries
./buildtool build

# Runs Bazel to test the project
./buildtool test

# Deletes data generated by Bazel and can clean the release artifacts
./buildtool clean

# Invokes Bazel to test the project
# Then invokes Bazel again to output binaries if tests passed
# Continues by packaging the generated binaries and the header files into a zip file to release on Github
./buildtool release --version=1.2.3 --os=darwin --arch=amd64
"""

import os
import shutil
import semver
import pathlib
import platform
try:
    import typer
except ImportError:
    print("Please install typer with `pip install \"typer[all]\"`.")
    exit(1)

from subprocess import run
try:
    from rich.progress import track
except ImportError:
    print("Please make sure rich is install alongside typer with `pip install \"typer[all]\"`.")
    exit(1)


app = typer.Typer(no_args_is_help = True)
libname = "ccollections"

# We get all available targets and tests from their directory names
targets = [f"{f.path}:{f.name}" for f in os.scandir("src/ccollections") if f.is_dir()]
tests = [f"//{f.path}:{f.name}_test" for f in os.scandir("tests/ccollections") if f.is_dir()]


# Courtesy of https://stackoverflow.com/questions/10349781/how-to-open-read-write-or-create-a-file-with-truncation-allowed/10352231#10352231
def _touchopen(filename, *args, **kwargs):
    fd = os.open(filename, os.O_RDWR | os.O_CREAT)
    return os.fdopen(fd, *args, **kwargs)


def _run(command, verbose, non_verbose_message = ""):
    """Runs Bazel to create output artifacts from targets specified in BUILD.bazel files."""
    number_of_failures = 0

    cmd_result = run(command, capture_output = True, text = True)
    
    if cmd_result.returncode != 0:
        number_of_failures += 1
        if verbose is True:
            print(cmd_result.stderr)
        else:
            print(non_verbose_message)

    return number_of_failures


@app.command()
def version(set_version: str = typer.Option(None, "--set", "-s")):
    """Sets and/or prints the current version of this project from the VERSION file at the root directory."""
    if set_version:
        if semver.VersionInfo.isvalid(set_version):
            with _touchopen("VERSION", "w+") as version_file:
                version_file.seek(0)
                version_file.write(set_version)
                version_file.truncate()

    with _touchopen("VERSION", 'r') as version_file:
        version = version_file.read()
        if not version:
            print(f"No version has been assigned to this project.")
            exit(1)
        
        print(version)


@app.command()
def about():
    """Simple wrapper around Bazel to help automate building, testing and creating releases for this project."""
    print(f"Simple wrapper around Bazel to help automate building, testing and creating releases for this project.")


@app.command()
def build(
    verbose: bool = typer.Option(False, "--verbose", "-v"),
    target: str = typer.Option(None, "--target", "-t")
):
    """Runs Bazel to create output artifacts from targets specified in BUILD.bazel files."""
    number_of_failures = 0

    if target is not None:
        target = f"src/ccollections/{target}:{target}"
        _targets = [target]
    else:
        global targets
        _targets = targets

    for target in track(_targets, description = "Building"):
        build_cmd = ["bazel", "build", f"//{target}"]
        number_of_failures += _run(
            build_cmd,
            verbose,
            f"Target {target} failed building. You can see the colorized output with `bazel build {target}`."
        )
    
    print(f"Built {len(_targets)} targets of which {number_of_failures} failed.")

    return number_of_failures


@app.command()
def test(target: str = typer.Option(None, "--target", "-t")):
    """Invokes Bazel to test the entire project."""
    number_of_failures = 0

    if target is not None:
        test = f"//tests/ccollections/{target}:{target}_test"
        _tests = [test]
    else:
        global tests
        _tests = tests

    for test in track(_tests, description = "Testing"):
        test_cmd = ["bazel", "test", "--test_output=all", test]
        number_of_failures += _run(
            test_cmd,
            True,
        )
    
    print(f"Ran {len(_tests)} tests out of which {number_of_failures} failed.")

    return number_of_failures


@app.command()
def clean(expunge: bool = typer.Option(False, "--expunge", "-e")):
    """Deletes all files generated by Bazel. If the --expunge option is provided, released artifacts, if any, are also deleted."""
    for file in os.listdir(os.getcwd()):
        if os.path.islink(file):
            os.unlink(file)

    if expunge:
        if os.path.isdir("release"):
            shutil.rmtree("release")

@app.command()
def release(
    version: str = typer.Option(None, "--version", "-v"),
    ops: str = typer.Option(None, "--os", "-o"),
    arch: str = typer.Option(None, "--arch", "-a")
):
    """
    \b
    Tests, builds then creates a zip file that contains the libraries and header files for distribution.

    \b
    A file named VERSION is opened to find the current version. This file should contain a single line of the semver of this library.
    If the file doesn't exist, the content of the version argument is used as the version and that content is written to VERSION.
    If the file exists, and the version argument is provided, the content of VERSION is replaced.
    If neither was provided, the release fails.
    
    \b
    If the operating system is not specified, it will determined automatically.
    Make sure it matches the one you know your system to be.
    The same applies to the architecture.
    
    \b
    In most cases, it will not be important to specify the operating system and architecture.
    But do so if this scripts get it wrong.
    """
    print(f"Attempting release of ccollections...")
    print()

    # If version is specified, make sure it is a valid semver
    if version and not semver.VersionInfo.isvalid(version):
        print(f"The given version \"{version}\" is not a valid semver.")
        exit(1)

    # Resolve the version to use for this project
    with _touchopen("VERSION", "r+") as version_file:
        content = version_file.read()
        if not semver.VersionInfo.isvalid(content):
            print(f"The version \"{content}\" found in VERSION is not a valid semver.")
            exit(1)
        
        if not content and not version:
            print("The VERSION file is empty, please provide a value for the --version option.")
            exit(1)

        # If a version is passed as an option, we replace the content of VERSION
        if not version:
            version = content
    
    # Resolve the operation system and architecture
    uname = platform.uname()
    
    # Set the release name
    release_name = "{}-{}-{}".format(libname, ops.lower() if ops else uname.system.lower(), arch.lower() if arch else uname.machine.lower())

    # Test the project and if tests fail, we don't continue further
    if test(None) > 0:
        exit(1)
    
    print()

    # Build the project and if building fails, we don't continue
    if build(False, None) > 0:
        exit(1)
    
    print()
    
    # Clean up existing release data
    release_folder = "release"
    if os.path.isdir(release_folder):
        shutil.rmtree(release_folder)
    
    # Create the bin path where everything will go -- this will create the needed parents as well
    ccollections_path = os.path.join("release", "ccollections")
    lib_path = os.path.join(ccollections_path, 'lib')
    lib_pathlib = pathlib.Path(lib_path)
    lib_pathlib.mkdir(parents = True, exist_ok = True)

    # Copy headers and artifacts
    for target in track(targets, description = "Releasing"):
        # Copy headers associated to this target
        header_file = target.replace(':', '/')
        header_file = f"{header_file}.h"
        header_dest = os.path.join(ccollections_path, os.path.basename(header_file))
        with _touchopen(header_dest, 'w'):
            shutil.copyfile(header_file, header_dest)

        # Copy artifacts associated to this target
        query_cmd = ["bazel", "cquery", target, "--output=files"]
        query_result = run(query_cmd, capture_output = True, text = True)
        artifacts = [y for y in (x.strip() for x in query_result.stdout.splitlines()) if y]
        for artifact in artifacts:
            artifact_dest = os.path.join(lib_path, os.path.basename(artifact))
            with _touchopen(artifact_dest, 'w'):
                shutil.copyfile(artifact, artifact_dest)

    # Generate the zip file to be released
    shutil.make_archive(os.path.join(release_folder, release_name), 'zip', ccollections_path)
    
    new_version = str(semver.VersionInfo.parse(version).bump_patch())
    with _touchopen("VERSION", "r+") as version_file:
        version_file.seek(0)
        version_file.write(new_version)
        version_file.truncate()
    
    
    print()

    print(f"Release finalized, new version is {libname} {new_version}.")

if __name__ == "__main__":
    app()
