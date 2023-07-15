{
    depfiles_gcc = "Hello.o: Hello.cpp\
",
    files = {
        "Hello.cpp"
    },
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-std=c++17"
        }
    }
}