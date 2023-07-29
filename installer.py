import os
import sys
import subprocess
import argparse
import shutil
import re

FILE_DIR = os.path.split(__file__)[0]
PUPPY_NAME = "module"
if sys.platform == "win32":
    LIB_EXT = ".dll"
elif sys.platform == "darwin":
    LIB_EXT = ".dylib"
else:
    LIB_EXT = ".so"


def stem(path: str):
    return ".".join(os.path.basename(path).split(".")[:-1])


def compile_source(source: str, out: str, use: str, generator: str):
    lists = os.path.join(source, "CMakeLists.txt")
    build = os.path.join(source, "build")
    cache = os.path.join(build, "CMakeCache.txt")
    library = os.path.join(build, "lib" + os.path.basename(source) + LIB_EXT)

    if not os.path.exists(lists):
        raise FileNotFoundError(
            "The source directory doesn't have a CMakeList.txt to compile.")
    if not os.path.exists(build):
        os.mkdir(build)

    with open(lists, "r") as file:
        content = old = file.read()
    content = re.sub(r"set\s*\(PUPS_PATH\s+.+?\)",
                     r"set(PUPS_PATH %s)" % os.path.abspath(use).replace("\\", "/"), content)
    with open(lists, "w") as file:
        file.write(content)
    if not os.path.exists(cache):
        subprocess.run(["cmake", "-S", source, "-B", build, "-G", generator])
    subprocess.run(["cmake", "--build", build])

    shutil.copy(library, out)
    with open(lists, "w") as file:
        file.write(old)


def compile_puppy(source: str, out: str, compiler: str):
    puppy = os.path.join(source, PUPPY_NAME + ".puppy")
    pups = os.path.join(out, os.path.basename(source) + ".pups")
    if not os.path.exists(puppy):
        raise FileNotFoundError(
            "The source directory doesn't have a %s.puppy to compile." % PUPPY_NAME)
    subprocess.run([sys.executable, compiler, puppy, "-o", pups])


def work(source: str, out: str, use: str, generator: str):
    compiler = os.path.join(use, "puppyscript", "main.py")
    if not os.path.exists(out):
        os.mkdir(out)

    print("Compiling CMake source...")
    compile_source(source, out, use, generator)
    print("Compiling puppy source...")
    compile_puppy(source, out, compiler)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=str,
                        help="The source library to install")
    parser.add_argument("--out", "-o", type=str,
                        help="Where to output the compiled library")
    parser.add_argument("--use", "-u", type=str,
                        help="The PourUPScript path that is used. Defaults to PourUPScript under the parent directory of this file.", default=None)
    parser.add_argument(
        "--generator", help="The generator that cmake uses. Defaults to \"MinGW Makefiles\"", default="MinGW Makefiles")
    args = parser.parse_args()
    source = args.source
    out = args.out
    use = args.use
    generator = args.generator
    if use is None:
        use = os.path.join(FILE_DIR, "PourUPScript")
    work(source, out, use, generator)
    print("Installing package %s completed." % os.path.basename(source))


if __name__ == "__main__":
    main()
