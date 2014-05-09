import qbs

Project {
    name: "emulator"
    references: [
        "internal_api/internal.qbs",
        "common/common.qbs",
        "core/core.qbs",
        "manager/manager.qbs",
        "launcher/launcher.qbs",
        "TAs/TrustedApplications.qbs",
        "tests/crypto_test.qbs",
        "tests/storage_test.qbs"
    ]
}
