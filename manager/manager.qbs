import qbs

DynamicLibrary {
    name: "ManagerApi"
    Depends { name: "cpp" }
    Depends { name: "CommonApi" }

    destinationDirectory: '.'

    cpp.dynamicLibraries: [
        "dl",
        "pthread"
    ]

    cpp.includePaths: [
        "../core",
        "../include",
    ]

    files: [
        "../core/core_extern_resources.h",
        "../core/main.c",
        "mainloop.c",
        "manager_extern_resources.h",
    ]
}
