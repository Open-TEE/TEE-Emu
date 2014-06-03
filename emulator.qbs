import qbs

Project {
    name: "emulator"
    references: [
        "internal_api/internal.qbs",
        "main/main.qbs",
        "common/common.qbs",
        "manager/manager.qbs",
        "launcher/launcher.qbs",
        "TAs/TrustedApplications.qbs",
        "tests/crypto_test.qbs",
        "tests/storage_test.qbs"
    ]
}
