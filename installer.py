import os
import sys
import subprocess
import argparse
import shutil
import re

FILE_DIR = os.path.split(__file__)[0]
if sys.platform == "win32":
    LIB_EXT = ".dll"
    DEFAULT_GENERATOR = "MinGW Makefiles"
    EXE_SUFFIX = ".exe"
elif sys.platform == "darwin":
    LIB_EXT = ".dylib"
    DEFAULT_GENERATOR = "Unix Makefiles"
    EXE_SUFFIX = ""
else:
    LIB_EXT = ".so"
    DEFAULT_GENERATOR = "Unix Makefiles"
    EXE_SUFFIX = ""


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


def compile_puppy(source: str, out: str, compiler: str, cpp_puppy: bool):
    basename = os.path.basename(source)
    puppy = os.path.join(source, basename + ".puppy")
    pups = os.path.join(out, basename + ".pups")
    if not os.path.exists(puppy):
        raise FileNotFoundError(
            "Connot find puppy configuration file \"%s\"." % puppy)
    if cpp_puppy:
        subprocess.run([compiler, puppy, "-o", pups])
    else:
        subprocess.run([sys.executable, compiler, puppy, "-o", pups])

def get_compiler(use: str, generator: str, cpp_puppy: bool):
    if cpp_puppy:
        source = os.path.join(use, "puppyscript", "cpp-puppy")
        lists = os.path.join(source, "CMakeLists.txt")
        if not os.path.exists(lists):
            raise FileNotFoundError(
                "Cannot find file \"%s\" required for cpp-puppy." % lists
            )
        build = os.path.join(source, "build")
        cache = os.path.join(build, "CMakeCache.txt")
        executable = os.path.join(build, "cpp_puppy") + EXE_SUFFIX
        if not os.path.exists(build):
            os.mkdir(build)
        if not os.path.exists(cache):
            subprocess.run(["cmake", "-S", source, "-B", build, "-G", generator])
        subprocess.run(["cmake", "--build", build])
        if not os.path.exists(executable):
            raise RuntimeError(
                "cpp-puppy compiler build failed."
            )
        return executable
    else:
        return os.path.join(use, "puppyscript", "main.py")


def work(source: str, out: str, use: str, generator: str, cpp_puppy: bool):
    compiler = get_compiler(use, generator, cpp_puppy)
    if not os.path.exists(out):
        os.mkdir(out)

    print("Compiling CMake source...")
    compile_source(source, out, use, generator)
    print("Compiling puppy source...")
    compile_puppy(source, out, compiler, cpp_puppy)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=str,
                        help="The source library to install")
    parser.add_argument("--out", "-o", type=str,
                        help="Where to output the compiled library")
    parser.add_argument("--use", "-u", type=str,
                        help="The PourUPScript path that is used. Defaults to PourUPScript under the parent directory of this file.", default=None)
    parser.add_argument(
        "--generator", help="The generator that cmake uses. Defaults to \"%s\"" % DEFAULT_GENERATOR, default=DEFAULT_GENERATOR)
    parser.add_argument(
        "--cpp-puppy", "--cpp", type=bool, default=False, nargs="?", const=True,
        help="Use the C++ alternative PuppyScript compiler."
    )
    args = parser.parse_args()
    source = args.source
    out = args.out
    use = args.use
    generator = args.generator
    cpp_puppy = args.cpp_puppy
    if use is None:
        use = os.path.join(FILE_DIR, "PourUPScript")
    work(source, out, use, generator, cpp_puppy)
    print("Package %s installed." % os.path.basename(source))


if __name__ == "__main__":
    main()
