#pragma once

cxex U1 PROCESS_PRIORITY_CLASS_HIGH = 3;

cxex SI RTL_MAX_DRIVE_LETTERS = 32;
cxex SI GDI_BATCH_BUFFER_SIZE = 310;
cxex SI WIN32_CLIENT_INFO_LENGTH = 62;
cxex SI STATIC_UNICODE_BUFFER_LENGTH = 261;

cxex SI TOKEN_USER_MAX_SIZE_ = 76;

enum class SYSTEM_INFORMATION_CLASS_
{
    SystemBasicInformation,                        // q: SYSTEM_BASIC_INFORMATION
    SystemProcessorInformation,                    // q: SYSTEM_PROCESSOR_INFORMATION
    SystemPerformanceInformation,                  // q: SYSTEM_PERFORMANCE_INFORMATION
    SystemTimeOfDayInformation,                    // q: SYSTEM_TIMEOFDAY_INFORMATION
    SystemPathInformation,                         // not implemented
    SystemProcessInformation,                      // q: SYSTEM_PROCESS_INFORMATION
    SystemCallCountInformation,                    // q: SYSTEM_CALL_COUNT_INFORMATION
    SystemDeviceInformation,                       // q: SYSTEM_DEVICE_INFORMATION
    SystemProcessorPerformanceInformation,         // q: SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION (EX in: USHORT ProcessorGroup)
    SystemFlagsInformation,                        // q: SYSTEM_FLAGS_INFORMATION
    SystemCallTimeInformation,                     // not implemented // SYSTEM_CALL_TIME_INFORMATION // 10
    SystemModuleInformation,                       // q: RTL_PROCESS_MODULES
    SystemLocksInformation,                        // q: RTL_PROCESS_LOCKS
    SystemStackTraceInformation,                   // q: RTL_PROCESS_BACKTRACES
    SystemPagedPoolInformation,                    // not implemented
    SystemNonPagedPoolInformation,                 // not implemented
    SystemHandleInformation,                       // q: SYSTEM_HANDLE_INFORMATION
    SystemObjectInformation,                       // q: SYSTEM_OBJECTTYPE_INFORMATION mixed with SYSTEM_OBJECT_INFORMATION
    SystemPageFileInformation,                     // q: SYSTEM_PAGEFILE_INFORMATION
    SystemVdmInstemulInformation,                  // q: SYSTEM_VDM_INSTEMUL_INFO
    SystemVdmBopInformation,                       // not implemented // 20
    SystemFileCacheInformation,                    // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypeSystemCache)
    SystemPoolTagInformation,                      // q: SYSTEM_POOLTAG_INFORMATION
    SystemInterruptInformation,                    // q: SYSTEM_INTERRUPT_INFORMATION (EX in: USHORT ProcessorGroup)
    SystemDpcBehaviorInformation,                  // q: SYSTEM_DPC_BEHAVIOR_INFORMATION; s: SYSTEM_DPC_BEHAVIOR_INFORMATION (requires SeLoadDriverPrivilege)
    SystemFullMemoryInformation,                   // not implemented // SYSTEM_MEMORY_USAGE_INFORMATION
    SystemLoadGdiDriverInformation,                // s (kernel-mode only)
    SystemUnloadGdiDriverInformation,              // s (kernel-mode only)
    SystemTimeAdjustmentInformation,               // q: SYSTEM_QUERY_TIME_ADJUST_INFORMATION; s: SYSTEM_SET_TIME_ADJUST_INFORMATION (requires SeSystemtimePrivilege)
    SystemSummaryMemoryInformation,                // not implemented // SYSTEM_MEMORY_USAGE_INFORMATION
    SystemMirrorMemoryInformation,                 // s (requires license value "Kernel-MemoryMirroringSupported") (requires SeShutdownPrivilege) // 30
    SystemPerformanceTraceInformation,             // q; s: (type depends on EVENT_TRACE_INFORMATION_CLASS)
    SystemObsolete0,                               // not implemented
    SystemExceptionInformation,                    // q: SYSTEM_EXCEPTION_INFORMATION
    SystemCrashDumpStateInformation,               // s: SYSTEM_CRASH_DUMP_STATE_INFORMATION (requires SeDebugPrivilege)
    SystemKernelDebuggerInformation,               // q: SYSTEM_KERNEL_DEBUGGER_INFORMATION
    SystemContextSwitchInformation,                // q: SYSTEM_CONTEXT_SWITCH_INFORMATION
    SystemRegistryQuotaInformation,                // q: SYSTEM_REGISTRY_QUOTA_INFORMATION; s (requires SeIncreaseQuotaPrivilege)
    SystemExtendServiceTableInformation,           // s (requires SeLoadDriverPrivilege) // loads win32k only
    SystemPrioritySeparation,                      // s (requires SeTcbPrivilege)
    SystemVerifierAddDriverInformation,            // s: UNICODE_STRING (requires SeDebugPrivilege) // 40
    SystemVerifierRemoveDriverInformation,         // s: UNICODE_STRING (requires SeDebugPrivilege)
    SystemProcessorIdleInformation,                // q: SYSTEM_PROCESSOR_IDLE_INFORMATION (EX in: USHORT ProcessorGroup)
    SystemLegacyDriverInformation,                 // q: SYSTEM_LEGACY_DRIVER_INFORMATION
    SystemCurrentTimeZoneInformation,              // q; s: RTL_TIME_ZONE_INFORMATION
    SystemLookasideInformation,                    // q: SYSTEM_LOOKASIDE_INFORMATION
    SystemTimeSlipNotification,                    // s: HANDLE (NtCreateEvent) (requires SeSystemtimePrivilege)
    SystemSessionCreate,                           // not implemented
    SystemSessionDetach,                           // not implemented
    SystemSessionInformation,                      // not implemented (SYSTEM_SESSION_INFORMATION)
    SystemRangeStartInformation,                   // q: SYSTEM_RANGE_START_INFORMATION // 50
    SystemVerifierInformation,                     // q: SYSTEM_VERIFIER_INFORMATION; s (requires SeDebugPrivilege)
    SystemVerifierThunkExtend,                     // s (kernel-mode only)
    SystemSessionProcessInformation,               // q: SYSTEM_SESSION_PROCESS_INFORMATION
    SystemLoadGdiDriverInSystemSpace,              // s: SYSTEM_GDI_DRIVER_INFORMATION (kernel-mode only) (same as SystemLoadGdiDriverInformation)
    SystemNumaProcessorMap,                        // q: SYSTEM_NUMA_INFORMATION
    SystemPrefetcherInformation,                   // q; s: PREFETCHER_INFORMATION // PfSnQueryPrefetcherInformation
    SystemExtendedProcessInformation,              // q: SYSTEM_EXTENDED_PROCESS_INFORMATION
    SystemRecommendedSharedDataAlignment,          // q: ULONG // KeGetRecommendedSharedDataAlignment
    SystemComPlusPackage,                          // q; s: ULONG
    SystemNumaAvailableMemory,                     // q: SYSTEM_NUMA_INFORMATION // 60
    SystemProcessorPowerInformation,               // q: SYSTEM_PROCESSOR_POWER_INFORMATION (EX in: USHORT ProcessorGroup)
    SystemEmulationBasicInformation,               // q: SYSTEM_BASIC_INFORMATION
    SystemEmulationProcessorInformation,           // q: SYSTEM_PROCESSOR_INFORMATION
    SystemExtendedHandleInformation,               // q: SYSTEM_HANDLE_INFORMATION_EX
    SystemLostDelayedWriteInformation,             // q: ULONG
    SystemBigPoolInformation,                      // q: SYSTEM_BIGPOOL_INFORMATION
    SystemSessionPoolTagInformation,               // q: SYSTEM_SESSION_POOLTAG_INFORMATION
    SystemSessionMappedViewInformation,            // q: SYSTEM_SESSION_MAPPED_VIEW_INFORMATION
    SystemHotpatchInformation,                     // q; s: SYSTEM_HOTPATCH_CODE_INFORMATION
    SystemObjectSecurityMode,                      // q: ULONG // 70
    SystemWatchdogTimerHandler,                    // s: SYSTEM_WATCHDOG_HANDLER_INFORMATION // (kernel-mode only)
    SystemWatchdogTimerInformation,                // q: SYSTEM_WATCHDOG_TIMER_INFORMATION // NtQuerySystemInformationEx // (kernel-mode only)
    SystemLogicalProcessorInformation,             // q: SYSTEM_LOGICAL_PROCESSOR_INFORMATION (EX in: USHORT ProcessorGroup) // NtQuerySystemInformationEx
    SystemWow64SharedInformationObsolete,          // not implemented
    SystemRegisterFirmwareTableInformationHandler, // s: SYSTEM_FIRMWARE_TABLE_HANDLER // (kernel-mode only)
    SystemFirmwareTableInformation,                // SYSTEM_FIRMWARE_TABLE_INFORMATION
    SystemModuleInformationEx,                     // q: RTL_PROCESS_MODULE_INFORMATION_EX // since VISTA
    SystemVerifierTriageInformation,               // not implemented
    SystemSuperfetchInformation,                   // q; s: SUPERFETCH_INFORMATION // PfQuerySuperfetchInformation
    SystemMemoryListInformation,                   // q: SYSTEM_MEMORY_LIST_INFORMATION; s: SYSTEM_MEMORY_LIST_COMMAND (requires SeProfileSingleProcessPrivilege) // 80
    SystemFileCacheInformationEx,                  // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (same as SystemFileCacheInformation)
    SystemThreadPriorityClientIdInformation,       // s: SYSTEM_THREAD_CID_PRIORITY_INFORMATION (requires SeIncreaseBasePriorityPrivilege) // NtQuerySystemInformationEx
    SystemProcessorIdleCycleTimeInformation,       // q: SYSTEM_PROCESSOR_IDLE_CYCLE_TIME_INFORMATION[] (EX in: USHORT ProcessorGroup) // NtQuerySystemInformationEx
    SystemVerifierCancellationInformation,         // SYSTEM_VERIFIER_CANCELLATION_INFORMATION // name:wow64:whNT32QuerySystemVerifierCancellationInformation
    SystemProcessorPowerInformationEx,             // not implemented
    SystemRefTraceInformation,                     // q; s: SYSTEM_REF_TRACE_INFORMATION // ObQueryRefTraceInformation
    SystemSpecialPoolInformation,                  // q; s: SYSTEM_SPECIAL_POOL_INFORMATION (requires SeDebugPrivilege) // MmSpecialPoolTag, then MmSpecialPoolCatchOverruns != 0
    SystemProcessIdInformation,                    // q: SYSTEM_PROCESS_ID_INFORMATION
    SystemErrorPortInformation,                    // s (requires SeTcbPrivilege)
    SystemBootEnvironmentInformation,              // q: SYSTEM_BOOT_ENVIRONMENT_INFORMATION // 90
    SystemHypervisorInformation,                   // q: SYSTEM_HYPERVISOR_QUERY_INFORMATION
    SystemVerifierInformationEx,                   // q; s: SYSTEM_VERIFIER_INFORMATION_EX
    SystemTimeZoneInformation,                     // q; s: RTL_TIME_ZONE_INFORMATION (requires SeTimeZonePrivilege)
    SystemImageFileExecutionOptionsInformation,    // s: SYSTEM_IMAGE_FILE_EXECUTION_OPTIONS_INFORMATION (requires SeTcbPrivilege)
    SystemCoverageInformation,                     // q: COVERAGE_MODULES s: COVERAGE_MODULE_REQUEST // ExpCovQueryInformation (requires SeDebugPrivilege)
    SystemPrefetchPatchInformation,                // SYSTEM_PREFETCH_PATCH_INFORMATION
    SystemVerifierFaultsInformation,               // s: SYSTEM_VERIFIER_FAULTS_INFORMATION (requires SeDebugPrivilege)
    SystemSystemPartitionInformation,              // q: SYSTEM_SYSTEM_PARTITION_INFORMATION
    SystemSystemDiskInformation,                   // q: SYSTEM_SYSTEM_DISK_INFORMATION
    SystemProcessorPerformanceDistribution,        // q: SYSTEM_PROCESSOR_PERFORMANCE_DISTRIBUTION (EX in: USHORT ProcessorGroup) // NtQuerySystemInformationEx // 100
    SystemNumaProximityNodeInformation,            // q; s: SYSTEM_NUMA_PROXIMITY_MAP
    SystemDynamicTimeZoneInformation,              // q; s: RTL_DYNAMIC_TIME_ZONE_INFORMATION (requires SeTimeZonePrivilege)
    SystemCodeIntegrityInformation,                // q: SYSTEM_CODEINTEGRITY_INFORMATION // SeCodeIntegrityQueryInformation
    SystemProcessorMicrocodeUpdateInformation,     // s: SYSTEM_PROCESSOR_MICROCODE_UPDATE_INFORMATION
    SystemProcessorBrandString,                    // q: CHAR[] // HaliQuerySystemInformation -> HalpGetProcessorBrandString, info class 23
    SystemVirtualAddressInformation,               // q: SYSTEM_VA_LIST_INFORMATION[]; s: SYSTEM_VA_LIST_INFORMATION[] (requires SeIncreaseQuotaPrivilege) // MmQuerySystemVaInformation
    SystemLogicalProcessorAndGroupInformation,     // q: SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX (EX in: LOGICAL_PROCESSOR_RELATIONSHIP RelationshipType) // since WIN7 // NtQuerySystemInformationEx //
                                                   // KeQueryLogicalProcessorRelationship
    SystemProcessorCycleTimeInformation,           // q: SYSTEM_PROCESSOR_CYCLE_TIME_INFORMATION[] (EX in: USHORT ProcessorGroup) // NtQuerySystemInformationEx
    SystemStoreInformation,                        // q; s: SYSTEM_STORE_INFORMATION (requires SeProfileSingleProcessPrivilege) // SmQueryStoreInformation
    SystemRegistryAppendString,                    // s: SYSTEM_REGISTRY_APPEND_STRING_PARAMETERS // 110
    SystemAitSamplingValue,                        // s: ULONG (requires SeProfileSingleProcessPrivilege)
    SystemVhdBootInformation,                      // q: SYSTEM_VHD_BOOT_INFORMATION
    SystemCpuQuotaInformation,                     // q; s: PS_CPU_QUOTA_QUERY_INFORMATION
    SystemNativeBasicInformation,                  // q: SYSTEM_BASIC_INFORMATION
    SystemErrorPortTimeouts,                       // SYSTEM_ERROR_PORT_TIMEOUTS
    SystemLowPriorityIoInformation,                // q: SYSTEM_LOW_PRIORITY_IO_INFORMATION
    SystemTpmBootEntropyInformation,               // q: BOOT_ENTROPY_NT_RESULT // ExQueryBootEntropyInformation
    SystemVerifierCountersInformation,             // q: SYSTEM_VERIFIER_COUNTERS_INFORMATION
    SystemPagedPoolInformationEx,                  // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypePagedPool)
    SystemSystemPtesInformationEx,                 // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypeSystemPtes) // 120
    SystemNodeDistanceInformation,                 // q: USHORT[4*NumaNodes] // (EX in: USHORT NodeNumber) // NtQuerySystemInformationEx
    SystemAcpiAuditInformation,                    // q: SYSTEM_ACPI_AUDIT_INFORMATION // HaliQuerySystemInformation -> HalpAuditQueryResults, info class 26
    SystemBasicPerformanceInformation,             // q: SYSTEM_BASIC_PERFORMANCE_INFORMATION // name:wow64:whNtQuerySystemInformation_SystemBasicPerformanceInformation
    SystemQueryPerformanceCounterInformation,      // q: SYSTEM_QUERY_PERFORMANCE_COUNTER_INFORMATION // since WIN7 SP1
    SystemSessionBigPoolInformation,               // q: SYSTEM_SESSION_POOLTAG_INFORMATION // since WIN8
    SystemBootGraphicsInformation,                 // q; s: SYSTEM_BOOT_GRAPHICS_INFORMATION (kernel-mode only)
    SystemScrubPhysicalMemoryInformation,          // q; s: MEMORY_SCRUB_INFORMATION
    SystemBadPageInformation,                      // SYSTEM_BAD_PAGE_INFORMATION
    SystemProcessorProfileControlArea,             // q; s: SYSTEM_PROCESSOR_PROFILE_CONTROL_AREA
    SystemCombinePhysicalMemoryInformation,        // s: MEMORY_COMBINE_INFORMATION, MEMORY_COMBINE_INFORMATION_EX, MEMORY_COMBINE_INFORMATION_EX2 // 130
    SystemEntropyInterruptTimingInformation,       // q; s: SYSTEM_ENTROPY_TIMING_INFORMATION
    SystemConsoleInformation,                      // q; s: SYSTEM_CONSOLE_INFORMATION
    SystemPlatformBinaryInformation,               // q: SYSTEM_PLATFORM_BINARY_INFORMATION (requires SeTcbPrivilege)
    SystemPolicyInformation,                       // q: SYSTEM_POLICY_INFORMATION (Warbird/Encrypt/Decrypt/Execute)
    SystemHypervisorProcessorCountInformation,     // q: SYSTEM_HYPERVISOR_PROCESSOR_COUNT_INFORMATION
    SystemDeviceDataInformation,                   // q: SYSTEM_DEVICE_DATA_INFORMATION
    SystemDeviceDataEnumerationInformation,        // q: SYSTEM_DEVICE_DATA_INFORMATION
    SystemMemoryTopologyInformation,               // q: SYSTEM_MEMORY_TOPOLOGY_INFORMATION
    SystemMemoryChannelInformation,                // q: SYSTEM_MEMORY_CHANNEL_INFORMATION
    SystemBootLogoInformation,                     // q: SYSTEM_BOOT_LOGO_INFORMATION // 140
    SystemProcessorPerformanceInformationEx,       // q: SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION_EX // (EX in: USHORT ProcessorGroup) // NtQuerySystemInformationEx // since WINBLUE
    SystemCriticalProcessErrorLogInformation,      // CRITICAL_PROCESS_EXCEPTION_DATA
    SystemSecureBootPolicyInformation,             // q: SYSTEM_SECUREBOOT_POLICY_INFORMATION
    SystemPageFileInformationEx,                   // q: SYSTEM_PAGEFILE_INFORMATION_EX
    SystemSecureBootInformation,                   // q: SYSTEM_SECUREBOOT_INFORMATION
    SystemEntropyInterruptTimingRawInformation,    // q; s: SYSTEM_ENTROPY_TIMING_INFORMATION
    SystemPortableWorkspaceEfiLauncherInformation, // q: SYSTEM_PORTABLE_WORKSPACE_EFI_LAUNCHER_INFORMATION
    SystemFullProcessInformation,                  // q: SYSTEM_EXTENDED_PROCESS_INFORMATION with SYSTEM_PROCESS_INFORMATION_EXTENSION (requires admin)
    SystemKernelDebuggerInformationEx,             // q: SYSTEM_KERNEL_DEBUGGER_INFORMATION_EX
    SystemBootMetadataInformation,                 // 150 // (requires SeTcbPrivilege)
    SystemSoftRebootInformation,                   // q: ULONG
    SystemElamCertificateInformation,              // s: SYSTEM_ELAM_CERTIFICATE_INFORMATION
    SystemOfflineDumpConfigInformation,            // q: OFFLINE_CRASHDUMP_CONFIGURATION_TABLE_V2
    SystemProcessorFeaturesInformation,            // q: SYSTEM_PROCESSOR_FEATURES_INFORMATION
    SystemRegistryReconciliationInformation,       // s: NULL (requires admin) (flushes registry hives)
    SystemEdidInformation,                         // q: SYSTEM_EDID_INFORMATION
    SystemManufacturingInformation,                // q: SYSTEM_MANUFACTURING_INFORMATION // since THRESHOLD
    SystemEnergyEstimationConfigInformation,       // q: SYSTEM_ENERGY_ESTIMATION_CONFIG_INFORMATION
    SystemHypervisorDetailInformation,             // q: SYSTEM_HYPERVISOR_DETAIL_INFORMATION
    SystemProcessorCycleStatsInformation,          // q: SYSTEM_PROCESSOR_CYCLE_STATS_INFORMATION (EX in: USHORT ProcessorGroup) // NtQuerySystemInformationEx // 160
    SystemVmGenerationCountInformation,
    SystemTrustedPlatformModuleInformation, // q: SYSTEM_TPM_INFORMATION
    SystemKernelDebuggerFlags,              // SYSTEM_KERNEL_DEBUGGER_FLAGS
    SystemCodeIntegrityPolicyInformation,   // q; s: SYSTEM_CODEINTEGRITYPOLICY_INFORMATION
    SystemIsolatedUserModeInformation,      // q: SYSTEM_ISOLATED_USER_MODE_INFORMATION
    SystemHardwareSecurityTestInterfaceResultsInformation,
    SystemSingleModuleInformation,         // q: SYSTEM_SINGLE_MODULE_INFORMATION
    SystemAllowedCpuSetsInformation,       // s: SYSTEM_WORKLOAD_ALLOWED_CPU_SET_INFORMATION
    SystemVsmProtectionInformation,        // q: SYSTEM_VSM_PROTECTION_INFORMATION (previously SystemDmaProtectionInformation)
    SystemInterruptCpuSetsInformation,     // q: SYSTEM_INTERRUPT_CPU_SET_INFORMATION // 170
    SystemSecureBootPolicyFullInformation, // q: SYSTEM_SECUREBOOT_POLICY_FULL_INFORMATION
    SystemCodeIntegrityPolicyFullInformation,
    SystemAffinitizedInterruptProcessorInformation, // q: KAFFINITY_EX // (requires SeIncreaseBasePriorityPrivilege)
    SystemRootSiloInformation,                      // q: SYSTEM_ROOT_SILO_INFORMATION
    SystemCpuSetInformation,                        // q: SYSTEM_CPU_SET_INFORMATION // since THRESHOLD2
    SystemCpuSetTagInformation,                     // q: SYSTEM_CPU_SET_TAG_INFORMATION
    SystemWin32WerStartCallout,
    SystemSecureKernelProfileInformation,           // q: SYSTEM_SECURE_KERNEL_HYPERGUARD_PROFILE_INFORMATION
    SystemCodeIntegrityPlatformManifestInformation, // q: SYSTEM_SECUREBOOT_PLATFORM_MANIFEST_INFORMATION // NtQuerySystemInformationEx // since REDSTONE
    SystemInterruptSteeringInformation,             // q: in: SYSTEM_INTERRUPT_STEERING_INFORMATION_INPUT, out: SYSTEM_INTERRUPT_STEERING_INFORMATION_OUTPUT // NtQuerySystemInformationEx // 180
    SystemSupportedProcessorArchitectures,          // p: in opt: HANDLE, out: SYSTEM_SUPPORTED_PROCESSOR_ARCHITECTURES_INFORMATION[] // NtQuerySystemInformationEx
    SystemMemoryUsageInformation,                   // q: SYSTEM_MEMORY_USAGE_INFORMATION
    SystemCodeIntegrityCertificateInformation,      // q: SYSTEM_CODEINTEGRITY_CERTIFICATE_INFORMATION
    SystemPhysicalMemoryInformation,                // q: SYSTEM_PHYSICAL_MEMORY_INFORMATION // since REDSTONE2
    SystemControlFlowTransition,                    // (Warbird/Encrypt/Decrypt/Execute)
    SystemKernelDebuggingAllowed,                   // s: ULONG
    SystemActivityModerationExeState,               // s: SYSTEM_ACTIVITY_MODERATION_EXE_STATE
    SystemActivityModerationUserSettings,           // q: SYSTEM_ACTIVITY_MODERATION_USER_SETTINGS
    SystemCodeIntegrityPoliciesFullInformation,     // NtQuerySystemInformationEx
    SystemCodeIntegrityUnlockInformation,           // SYSTEM_CODEINTEGRITY_UNLOCK_INFORMATION // 190
    SystemIntegrityQuotaInformation,
    SystemFlushInformation,                // q: SYSTEM_FLUSH_INFORMATION
    SystemProcessorIdleMaskInformation,    // q: ULONG_PTR[ActiveGroupCount] // since REDSTONE3
    SystemSecureDumpEncryptionInformation, // NtQuerySystemInformationEx
    SystemWriteConstraintInformation,      // SYSTEM_WRITE_CONSTRAINT_INFORMATION
    SystemKernelVaShadowInformation,       // SYSTEM_KERNEL_VA_SHADOW_INFORMATION
    SystemHypervisorSharedPageInformation, // SYSTEM_HYPERVISOR_SHARED_PAGE_INFORMATION // since REDSTONE4
    SystemFirmwareBootPerformanceInformation,
    SystemCodeIntegrityVerificationInformation,   // SYSTEM_CODEINTEGRITYVERIFICATION_INFORMATION
    SystemFirmwarePartitionInformation,           // SYSTEM_FIRMWARE_PARTITION_INFORMATION // 200
    SystemSpeculationControlInformation,          // SYSTEM_SPECULATION_CONTROL_INFORMATION // (CVE-2017-5715) REDSTONE3 and above.
    SystemDmaGuardPolicyInformation,              // SYSTEM_DMA_GUARD_POLICY_INFORMATION
    SystemEnclaveLaunchControlInformation,        // SYSTEM_ENCLAVE_LAUNCH_CONTROL_INFORMATION
    SystemWorkloadAllowedCpuSetsInformation,      // SYSTEM_WORKLOAD_ALLOWED_CPU_SET_INFORMATION // since REDSTONE5
    SystemCodeIntegrityUnlockModeInformation,     // SYSTEM_CODEINTEGRITY_UNLOCK_INFORMATION
    SystemLeapSecondInformation,                  // SYSTEM_LEAP_SECOND_INFORMATION
    SystemFlags2Information,                      // q: SYSTEM_FLAGS_INFORMATION
    SystemSecurityModelInformation,               // SYSTEM_SECURITY_MODEL_INFORMATION // since 19H1
    SystemCodeIntegritySyntheticCacheInformation, // NtQuerySystemInformationEx
    SystemFeatureConfigurationInformation,        // q: in: SYSTEM_FEATURE_CONFIGURATION_QUERY, out: SYSTEM_FEATURE_CONFIGURATION_INFORMATION; s: SYSTEM_FEATURE_CONFIGURATION_UPDATE //
                                                  // NtQuerySystemInformationEx // since 20H1 // 210
    SystemFeatureConfigurationSectionInformation, // q: in: SYSTEM_FEATURE_CONFIGURATION_SECTIONS_REQUEST, out: SYSTEM_FEATURE_CONFIGURATION_SECTIONS_INFORMATION // NtQuerySystemInformationEx
    SystemFeatureUsageSubscriptionInformation,    // q: SYSTEM_FEATURE_USAGE_SUBSCRIPTION_DETAILS; s: SYSTEM_FEATURE_USAGE_SUBSCRIPTION_UPDATE
    SystemSecureSpeculationControlInformation,    // SECURE_SPECULATION_CONTROL_INFORMATION
    SystemSpacesBootInformation,                  // since 20H2
    SystemFwRamdiskInformation,                   // SYSTEM_FIRMWARE_RAMDISK_INFORMATION
    SystemWheaIpmiHardwareInformation,
    SystemDifSetRuleClassInformation,          // s: SYSTEM_DIF_VOLATILE_INFORMATION (requires SeDebugPrivilege)
    SystemDifClearRuleClassInformation,        // s: NULL (requires SeDebugPrivilege)
    SystemDifApplyPluginVerificationOnDriver,  // SYSTEM_DIF_PLUGIN_DRIVER_INFORMATION (requires SeDebugPrivilege)
    SystemDifRemovePluginVerificationOnDriver, // SYSTEM_DIF_PLUGIN_DRIVER_INFORMATION (requires SeDebugPrivilege) // 220
    SystemShadowStackInformation,              // SYSTEM_SHADOW_STACK_INFORMATION
    SystemBuildVersionInformation,             // q: in: ULONG (LayerNumber), out: SYSTEM_BUILD_VERSION_INFORMATION // NtQuerySystemInformationEx // 222
    SystemPoolLimitInformation,                // SYSTEM_POOL_LIMIT_INFORMATION (requires SeIncreaseQuotaPrivilege) // NtQuerySystemInformationEx
    SystemCodeIntegrityAddDynamicStore,        // CodeIntegrity-AllowConfigurablePolicy-CustomKernelSigners
    SystemCodeIntegrityClearDynamicStores,     // CodeIntegrity-AllowConfigurablePolicy-CustomKernelSigners
    SystemDifPoolTrackingInformation,
    SystemPoolZeroingInformation,                 // q: SYSTEM_POOL_ZEROING_INFORMATION
    SystemDpcWatchdogInformation,                 // q; s: SYSTEM_DPC_WATCHDOG_CONFIGURATION_INFORMATION
    SystemDpcWatchdogInformation2,                // q; s: SYSTEM_DPC_WATCHDOG_CONFIGURATION_INFORMATION_V2
    SystemSupportedProcessorArchitectures2,       // q: in opt: HANDLE, out: SYSTEM_SUPPORTED_PROCESSOR_ARCHITECTURES_INFORMATION[] // NtQuerySystemInformationEx // 230
    SystemSingleProcessorRelationshipInformation, // q: SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX // (EX in: PROCESSOR_NUMBER Processor) // NtQuerySystemInformationEx
    SystemXfgCheckFailureInformation,             // q: SYSTEM_XFG_FAILURE_INFORMATION
    SystemIommuStateInformation,                  // SYSTEM_IOMMU_STATE_INFORMATION // since 22H1
    SystemHypervisorMinrootInformation,           // SYSTEM_HYPERVISOR_MINROOT_INFORMATION
    SystemHypervisorBootPagesInformation,         // SYSTEM_HYPERVISOR_BOOT_PAGES_INFORMATION
    SystemPointerAuthInformation,                 // SYSTEM_POINTER_AUTH_INFORMATION
    SystemSecureKernelDebuggerInformation,        // NtQuerySystemInformationEx
    SystemOriginalImageFeatureInformation,        // q: in: SYSTEM_ORIGINAL_IMAGE_FEATURE_INFORMATION_INPUT, out: SYSTEM_ORIGINAL_IMAGE_FEATURE_INFORMATION_OUTPUT // NtQuerySystemInformationEx
    SystemMemoryNumaInformation,                  // SYSTEM_MEMORY_NUMA_INFORMATION_INPUT, SYSTEM_MEMORY_NUMA_INFORMATION_OUTPUT // NtQuerySystemInformationEx
    SystemMemoryNumaPerformanceInformation, // SYSTEM_MEMORY_NUMA_PERFORMANCE_INFORMATION_INPUTSYSTEM_MEMORY_NUMA_PERFORMANCE_INFORMATION_INPUT, SYSTEM_MEMORY_NUMA_PERFORMANCE_INFORMATION_OUTPUT //
                                            // since 24H2 // 240
    SystemCodeIntegritySignedPoliciesFullInformation,
    SystemSecureCoreInformation,                    // SystemSecureSecretsInformation
    SystemTrustedAppsRuntimeInformation,            // SYSTEM_TRUSTEDAPPS_RUNTIME_INFORMATION
    SystemBadPageInformationEx,                     // SYSTEM_BAD_PAGE_INFORMATION
    SystemResourceDeadlockTimeout,                  // ULONG
    SystemBreakOnContextUnwindFailureInformation,   // ULONG (requires SeDebugPrivilege)
    SystemOslRamdiskInformation,                    // SYSTEM_OSL_RAMDISK_INFORMATION
    SystemCodeIntegrityPolicyManagementInformation, // SYSTEM_CODEINTEGRITYPOLICY_MANAGEMENT // since 25H2
    SystemMemoryNumaCacheInformation,
    SystemProcessorFeaturesBitMapInformation, // 250
    SystemRefTraceInformationEx,              // SYSTEM_REF_TRACE_INFORMATION_EX
    SystemBasicProcessInformation,            // SYSTEM_BASICPROCESS_INFORMATION
    SystemHandleCountInformation,             // SYSTEM_HANDLECOUNT_INFORMATION
    MaxSystemInfoClass
};
enum class PROCESSINFOCLASS_
{
    ProcessBasicInformation,                     // q: PROCESS_BASIC_INFORMATION, PROCESS_EXTENDED_BASIC_INFORMATION
    ProcessQuotaLimits,                          // qs: QUOTA_LIMITS, QUOTA_LIMITS_EX
    ProcessIoCounters,                           // q: IO_COUNTERS
    ProcessVmCounters,                           // q: VM_COUNTERS, VM_COUNTERS_EX, VM_COUNTERS_EX2
    ProcessTimes,                                // q: KERNEL_USER_TIMES
    ProcessBasePriority,                         // s: KPRIORITY
    ProcessRaisePriority,                        // s: ULONG
    ProcessDebugPort,                            // q: HANDLE
    ProcessExceptionPort,                        // s: PROCESS_EXCEPTION_PORT (requires SeTcbPrivilege)
    ProcessAccessToken,                          // s: PROCESS_ACCESS_TOKEN
    ProcessLdtInformation,                       // qs: PROCESS_LDT_INFORMATION // 10
    ProcessLdtSize,                              // s: PROCESS_LDT_SIZE
    ProcessDefaultHardErrorMode,                 // qs: ULONG
    ProcessIoPortHandlers,                       // s: PROCESS_IO_PORT_HANDLER_INFORMATION // (kernel-mode only)
    ProcessPooledUsageAndLimits,                 // q: POOLED_USAGE_AND_LIMITS
    ProcessWorkingSetWatch,                      // q: PROCESS_WS_WATCH_INFORMATION[]; s: void
    ProcessUserModeIOPL,                         // qs: ULONG (requires SeTcbPrivilege)
    ProcessEnableAlignmentFaultFixup,            // s: BOOLEAN
    ProcessPriorityClass,                        // qs: PROCESS_PRIORITY_CLASS
    ProcessWx86Information,                      // qs: ULONG (requires SeTcbPrivilege) (VdmAllowed)
    ProcessHandleCount,                          // q: ULONG, PROCESS_HANDLE_INFORMATION // 20
    ProcessAffinityMask,                         // (q >WIN7)s: KAFFINITY, qs: GROUP_AFFINITY
    ProcessPriorityBoost,                        // qs: ULONG
    ProcessDeviceMap,                            // qs: PROCESS_DEVICEMAP_INFORMATION, PROCESS_DEVICEMAP_INFORMATION_EX
    ProcessSessionInformation,                   // q: PROCESS_SESSION_INFORMATION
    ProcessForegroundInformation,                // s: PROCESS_FOREGROUND_BACKGROUND
    ProcessWow64Information,                     // q: ULONG_PTR
    ProcessImageFileName,                        // q: UNICODE_STRING
    ProcessLUIDDeviceMapsEnabled,                // q: ULONG
    ProcessBreakOnTermination,                   // qs: ULONG
    ProcessDebugObjectHandle,                    // q: HANDLE // 30
    ProcessDebugFlags,                           // qs: ULONG
    ProcessHandleTracing,                        // q: PROCESS_HANDLE_TRACING_QUERY; s: PROCESS_HANDLE_TRACING_ENABLE[_EX] or void to disable
    ProcessIoPriority,                           // qs: IO_PRIORITY_HINT
    ProcessExecuteFlags,                         // qs: ULONG (MEM_EXECUTE_OPTION_*)
    ProcessTlsInformation,                       // PROCESS_TLS_INFORMATION // ProcessResourceManagement
    ProcessCookie,                               // q: ULONG
    ProcessImageInformation,                     // q: SECTION_IMAGE_INFORMATION
    ProcessCycleTime,                            // q: PROCESS_CYCLE_TIME_INFORMATION // since VISTA
    ProcessPagePriority,                         // qs: PAGE_PRIORITY_INFORMATION
    ProcessInstrumentationCallback,              // s: PVOID or PROCESS_INSTRUMENTATION_CALLBACK_INFORMATION // 40
    ProcessThreadStackAllocation,                // s: PROCESS_STACK_ALLOCATION_INFORMATION, PROCESS_STACK_ALLOCATION_INFORMATION_EX
    ProcessWorkingSetWatchEx,                    // q: PROCESS_WS_WATCH_INFORMATION_EX[]; s: void
    ProcessImageFileNameWin32,                   // q: UNICODE_STRING
    ProcessImageFileMapping,                     // q: HANDLE (input)
    ProcessAffinityUpdateMode,                   // qs: PROCESS_AFFINITY_UPDATE_MODE
    ProcessMemoryAllocationMode,                 // qs: PROCESS_MEMORY_ALLOCATION_MODE
    ProcessGroupInformation,                     // q: USHORT[]
    ProcessTokenVirtualizationEnabled,           // s: ULONG
    ProcessConsoleHostProcess,                   // qs: ULONG_PTR // ProcessOwnerInformation
    ProcessWindowInformation,                    // q: PROCESS_WINDOW_INFORMATION // 50
    ProcessHandleInformation,                    // q: PROCESS_HANDLE_SNAPSHOT_INFORMATION // since WIN8
    ProcessMitigationPolicy,                     // s: PROCESS_MITIGATION_POLICY_INFORMATION
    ProcessDynamicFunctionTableInformation,      // s: PROCESS_DYNAMIC_FUNCTION_TABLE_INFORMATION
    ProcessHandleCheckingMode,                   // qs: ULONG; s: 0 disables, otherwise enables
    ProcessKeepAliveCount,                       // q: PROCESS_KEEPALIVE_COUNT_INFORMATION
    ProcessRevokeFileHandles,                    // s: PROCESS_REVOKE_FILE_HANDLES_INFORMATION
    ProcessWorkingSetControl,                    // s: PROCESS_WORKING_SET_CONTROL
    ProcessHandleTable,                          // q: ULONG[] // since WINBLUE
    ProcessCheckStackExtentsMode,                // qs: ULONG // KPROCESS->CheckStackExtents (CFG)
    ProcessCommandLineInformation,               // q: UNICODE_STRING // 60
    ProcessProtectionInformation,                // q: PS_PROTECTION
    ProcessMemoryExhaustion,                     // s: PROCESS_MEMORY_EXHAUSTION_INFO // since THRESHOLD
    ProcessFaultInformation,                     // s: PROCESS_FAULT_INFORMATION
    ProcessTelemetryIdInformation,               // q: PROCESS_TELEMETRY_ID_INFORMATION
    ProcessCommitReleaseInformation,             // qs: PROCESS_COMMIT_RELEASE_INFORMATION
    ProcessDefaultCpuSetsInformation,            // qs: SYSTEM_CPU_SET_INFORMATION[5]
    ProcessAllowedCpuSetsInformation,            // qs: SYSTEM_CPU_SET_INFORMATION[5]
    ProcessSubsystemProcess,                     // s: void // EPROCESS->SubsystemProcess
    ProcessJobMemoryInformation,                 // q: PROCESS_JOB_MEMORY_INFO
    ProcessInPrivate,                            // q: BOOLEAN; s: void // ETW // since THRESHOLD2 // 70
    ProcessRaiseUMExceptionOnInvalidHandleClose, // qs: ULONG; s: 0 disables, otherwise enables
    ProcessIumChallengeResponse,
    ProcessChildProcessInformation,         // q: PROCESS_CHILD_PROCESS_INFORMATION
    ProcessHighGraphicsPriorityInformation, // qs: BOOLEAN (requires SeTcbPrivilege)
    ProcessSubsystemInformation,            // q: SUBSYSTEM_INFORMATION_TYPE // since REDSTONE2
    ProcessEnergyValues,                    // q: PROCESS_ENERGY_VALUES, PROCESS_EXTENDED_ENERGY_VALUES, PROCESS_EXTENDED_ENERGY_VALUES_V1
    ProcessPowerThrottlingState,            // qs: POWER_THROTTLING_PROCESS_STATE
    ProcessReserved3Information,            // ProcessActivityThrottlePolicy // PROCESS_ACTIVITY_THROTTLE_POLICY
    ProcessWin32kSyscallFilterInformation,  // q: WIN32K_SYSCALL_FILTER
    ProcessDisableSystemAllowedCpuSets,     // s: BOOLEAN // 80
    ProcessWakeInformation,                 // q: PROCESS_WAKE_INFORMATION
    ProcessEnergyTrackingState,             // qs: PROCESS_ENERGY_TRACKING_STATE
    ProcessManageWritesToExecutableMemory,  // MANAGE_WRITES_TO_EXECUTABLE_MEMORY // since REDSTONE3
    ProcessCaptureTrustletLiveDump,         // q: ULONG
    ProcessTelemetryCoverage,               // q: TELEMETRY_COVERAGE_HEADER; s: TELEMETRY_COVERAGE_POINT
    ProcessEnclaveInformation,
    ProcessEnableReadWriteVmLogging,           // qs: PROCESS_READWRITEVM_LOGGING_INFORMATION
    ProcessUptimeInformation,                  // q: PROCESS_UPTIME_INFORMATION
    ProcessImageSection,                       // q: HANDLE
    ProcessDebugAuthInformation,               // s: CiTool.exe --device-id // PplDebugAuthorization // since RS4 // 90
    ProcessSystemResourceManagement,           // s: PROCESS_SYSTEM_RESOURCE_MANAGEMENT
    ProcessSequenceNumber,                     // q: ULONGLONG
    ProcessLoaderDetour,                       // since RS5
    ProcessSecurityDomainInformation,          // q: PROCESS_SECURITY_DOMAIN_INFORMATION
    ProcessCombineSecurityDomainsInformation,  // s: PROCESS_COMBINE_SECURITY_DOMAINS_INFORMATION
    ProcessEnableLogging,                      // qs: PROCESS_LOGGING_INFORMATION
    ProcessLeapSecondInformation,              // qs: PROCESS_LEAP_SECOND_INFORMATION
    ProcessFiberShadowStackAllocation,         // s: PROCESS_FIBER_SHADOW_STACK_ALLOCATION_INFORMATION // since 19H1
    ProcessFreeFiberShadowStackAllocation,     // s: PROCESS_FREE_FIBER_SHADOW_STACK_ALLOCATION_INFORMATION
    ProcessAltSystemCallInformation,           // s: PROCESS_SYSCALL_PROVIDER_INFORMATION // since 20H1 // 100
    ProcessDynamicEHContinuationTargets,       // s: PROCESS_DYNAMIC_EH_CONTINUATION_TARGETS_INFORMATION
    ProcessDynamicEnforcedCetCompatibleRanges, // s: PROCESS_DYNAMIC_ENFORCED_ADDRESS_RANGE_INFORMATION // since 20H2
    ProcessCreateStateChange,                  // since WIN11
    ProcessApplyStateChange,
    ProcessEnableOptionalXStateFeatures, // s: ULONG64 // optional XState feature bitmask
    ProcessAltPrefetchParam,             // qs: OVERRIDE_PREFETCH_PARAMETER // App Launch Prefetch (ALPF) // since 22H1
    ProcessAssignCpuPartitions,          // HANDLE
    ProcessPriorityClassEx,              // s: PROCESS_PRIORITY_CLASS_EX
    ProcessMembershipInformation,        // q: PROCESS_MEMBERSHIP_INFORMATION
    ProcessEffectiveIoPriority,          // q: IO_PRIORITY_HINT // 110
    ProcessEffectivePagePriority,        // q: ULONG
    ProcessSchedulerSharedData,          // SCHEDULER_SHARED_DATA_SLOT_INFORMATION // since 24H2
    ProcessSlistRollbackInformation,
    ProcessNetworkIoCounters,              // q: PROCESS_NETWORK_COUNTERS
    ProcessFindFirstThreadByTebValue,      // PROCESS_TEB_VALUE_INFORMATION
    ProcessEnclaveAddressSpaceRestriction, // since 25H2
    ProcessAvailableCpus,                  // PROCESS_AVAILABLE_CPUS_INFORMATION
    MaxProcessInfoClass
};
enum class SECURITY_IMPERSONATION_LEVEL_
{
    SecurityAnonymous,
    SecurityIdentification,
    SecurityImpersonation,
    SecurityDelegation
};
enum class NT_PRODUCT_TYPE_
{
    NtProductWinNt = 1,
    NtProductLanManNt,
    NtProductServer
};
enum class LDR_DLL_LOAD_REASON_
{
    LoadReasonStaticDependency,
    LoadReasonStaticForwarderDependency,
    LoadReasonDynamicForwarderDependency,
    LoadReasonDelayloadDependency,
    LoadReasonDynamicLoad,
    LoadReasonAsImageLoad,
    LoadReasonAsDataLoad,
    LoadReasonEnclavePrimary,
    LoadReasonEnclaveDependency,
    LoadReasonPatchImage,
    LoadReasonUnknown = -1
};
enum class LDR_HOT_PATCH_STATE_
{
    LdrHotPatchBaseImage,
    LdrHotPatchNotApplied,
    LdrHotPatchAppliedReverse,
    LdrHotPatchAppliedForward,
    LdrHotPatchFailedToPatch,
    LdrHotPatchStateMax,
};
enum class LDR_DDAG_STATE_
{
    LdrModulesMerged = -5,
    LdrModulesInitError = -4,
    LdrModulesSnapError = -3,
    LdrModulesUnloaded = -2,
    LdrModulesUnloading = -1,
    LdrModulesPlaceHolder = 0,
    LdrModulesMapping = 1,
    LdrModulesMapped = 2,
    LdrModulesWaitingForDependencies = 3,
    LdrModulesSnapping = 4,
    LdrModulesSnapped = 5,
    LdrModulesCondensed = 6,
    LdrModulesReadyToInit = 7,
    LdrModulesInitializing = 8,
    LdrModulesReadyToRun = 9
};
enum class EXCEPTION_DISPOSITION_
{
    ExceptionContinueExecution_ = 0, // with _ postfix to avoid conflict
    ExceptionContinueSearch_ = 1,    // with _ postfix to avoid conflict
    ExceptionNestedException_ = 2,   // with _ postfix to avoid conflict
    ExceptionCollidedUnwind_ = 3,    // with _ postfix to avoid conflict
};
enum class TOKEN_INFORMATION_CLASS_
{
    TokenUser = 1,                        // q: TOKEN_USER, SE_TOKEN_USER
    TokenGroups,                          // q: TOKEN_GROUPS
    TokenPrivileges,                      // q: TOKEN_PRIVILEGES
    TokenOwner,                           // q; s: TOKEN_OWNER
    TokenPrimaryGroup,                    // q; s: TOKEN_PRIMARY_GROUP
    TokenDefaultDacl,                     // q; s: TOKEN_DEFAULT_DACL
    TokenSource,                          // q: TOKEN_SOURCE
    TokenType,                            // q: TOKEN_TYPE
    TokenImpersonationLevel,              // q: SECURITY_IMPERSONATION_LEVEL
    TokenStatistics,                      // q: TOKEN_STATISTICS // 10
    TokenRestrictedSids,                  // q: TOKEN_GROUPS
    TokenSessionId,                       // q; s: ULONG (requires SeTcbPrivilege)
    TokenGroupsAndPrivileges,             // q: TOKEN_GROUPS_AND_PRIVILEGES
    TokenSessionReference,                // s: ULONG (requires SeTcbPrivilege)
    TokenSandBoxInert,                    // q: ULONG
    TokenAuditPolicy,                     // q; s: TOKEN_AUDIT_POLICY (requires SeSecurityPrivilege/SeTcbPrivilege)
    TokenOrigin,                          // q; s: TOKEN_ORIGIN (requires SeTcbPrivilege)
    TokenElevationType,                   // q: TOKEN_ELEVATION_TYPE
    TokenLinkedToken,                     // q; s: TOKEN_LINKED_TOKEN (requires SeCreateTokenPrivilege)
    TokenElevation,                       // q: TOKEN_ELEVATION // 20
    TokenHasRestrictions,                 // q: ULONG
    TokenAccessInformation,               // q: TOKEN_ACCESS_INFORMATION
    TokenVirtualizationAllowed,           // q; s: ULONG (requires SeCreateTokenPrivilege)
    TokenVirtualizationEnabled,           // q; s: ULONG
    TokenIntegrityLevel,                  // q; s: TOKEN_MANDATORY_LABEL
    TokenUIAccess,                        // q; s: ULONG (requires SeTcbPrivilege)
    TokenMandatoryPolicy,                 // q; s: TOKEN_MANDATORY_POLICY (requires SeTcbPrivilege)
    TokenLogonSid,                        // q: TOKEN_GROUPS
    TokenIsAppContainer,                  // q: ULONG // since WIN8
    TokenCapabilities,                    // q: TOKEN_GROUPS // 30
    TokenAppContainerSid,                 // q: TOKEN_APPCONTAINER_INFORMATION
    TokenAppContainerNumber,              // q: ULONG
    TokenUserClaimAttributes,             // q: CLAIM_SECURITY_ATTRIBUTES_INFORMATION
    TokenDeviceClaimAttributes,           // q: CLAIM_SECURITY_ATTRIBUTES_INFORMATION
    TokenRestrictedUserClaimAttributes,   // q: CLAIM_SECURITY_ATTRIBUTES_INFORMATION
    TokenRestrictedDeviceClaimAttributes, // q: CLAIM_SECURITY_ATTRIBUTES_INFORMATION
    TokenDeviceGroups,                    // q: TOKEN_GROUPS
    TokenRestrictedDeviceGroups,          // q: TOKEN_GROUPS
    TokenSecurityAttributes,              // q; s: TOKEN_SECURITY_ATTRIBUTES_[AND_OPERATION_]INFORMATION (requires SeTcbPrivilege)
    TokenIsRestricted,                    // q: ULONG // 40
    TokenProcessTrustLevel,               // q: TOKEN_PROCESS_TRUST_LEVEL // since WINBLUE
    TokenPrivateNameSpace,                // q; s: ULONG (requires SeTcbPrivilege) // since THRESHOLD
    TokenSingletonAttributes,             // q: TOKEN_SECURITY_ATTRIBUTES_INFORMATION // since REDSTONE
    TokenBnoIsolation,                    // q: TOKEN_BNO_ISOLATION_INFORMATION // since REDSTONE2
    TokenChildProcessFlags,               // s: ULONG  (requires SeTcbPrivilege) // since REDSTONE3
    TokenIsLessPrivilegedAppContainer,    // q: ULONG // since REDSTONE5
    TokenIsSandboxed,                     // q: ULONG // since 19H1
    TokenIsAppSilo,                       // q: ULONG // since WIN11 22H2 // previously TokenOriginatingProcessTrustLevel // q: TOKEN_PROCESS_TRUST_LEVEL
    TokenLoggingInformation,              // TOKEN_LOGGING_INFORMATION // since 24H2
    TokenLearningMode,                    // since 25H2
    MaxTokenInfoClass
};
enum class TOKEN_TYPE_
{
    TokenPrimary = 1,
    TokenImpersonation
};
enum class OBJECT_INFORMATION_CLASS_
{
    ObjectBasicInformation,         // q: OBJECT_BASIC_INFORMATION
    ObjectNameInformation,          // q: OBJECT_NAME_INFORMATION
    ObjectTypeInformation,          // q: OBJECT_TYPE_INFORMATION
    ObjectTypesInformation,         // q: OBJECT_TYPES_INFORMATION
    ObjectHandleFlagInformation,    // qs: OBJECT_HANDLE_FLAG_INFORMATION
    ObjectSessionInformation,       // s: void // change object session // (requires SeTcbPrivilege)
    ObjectSessionObjectInformation, // s: void // change object session // (requires SeTcbPrivilege)
    ObjectSetRefTraceInformation,   // since 25H2
    MaxObjectInfoClass
};
enum class PS_CREATE_STATE_
{
    PsCreateInitialState,
    PsCreateFailOnFileOpen,
    PsCreateFailOnSectionCreate,
    PsCreateFailExeFormat,
    PsCreateFailMachineMismatch,
    PsCreateFailExeName, // Debugger specified
    PsCreateSuccess,
    PsCreateMaximumStates
};
enum class KTHREAD_STATE_
{
    Initialized,
    Ready,
    Running,
    Standby,
    Terminated,
    Waiting,
    Transition,
    DeferredReady,
    GateWaitObsolete,
    WaitingForProcessInSwap,
    MaximumThreadState
};
enum class KWAIT_REASON_
{
    Executive,         // Waiting for an executive event.
    FreePage,          // Waiting for a free page.
    PageIn,            // Waiting for a page to be read in.
    PoolAllocation,    // Waiting for a pool allocation.
    DelayExecution,    // Waiting due to a delay execution.           // NtDelayExecution
    Suspended,         // Waiting because the thread is suspended.    // NtSuspendThread
    UserRequest,       // Waiting due to a user request.              // NtWaitForSingleObject
    WrExecutive,       // Waiting for an executive event.
    WrFreePage,        // Waiting for a free page.
    WrPageIn,          // Waiting for a page to be read in.
    WrPoolAllocation,  // Waiting for a pool allocation.              // 10
    WrDelayExecution,  // Waiting due to a delay execution.
    WrSuspended,       // Waiting because the thread is suspended.
    WrUserRequest,     // Waiting due to a user request.
    WrEventPair,       // Waiting for an event pair.                  // NtCreateEventPair
    WrQueue,           // Waiting for a queue.                        // NtRemoveIoCompletion
    WrLpcReceive,      // Waiting for an LPC receive.                 // NtReplyWaitReceivePort
    WrLpcReply,        // Waiting for an LPC reply.                   // NtRequestWaitReplyPort
    WrVirtualMemory,   // Waiting for virtual memory.
    WrPageOut,         // Waiting for a page to be written out.       // NtFlushVirtualMemory
    WrRendezvous,      // Waiting for a rendezvous.                   // 20
    WrKeyedEvent,      // Waiting for a keyed event.                  // NtCreateKeyedEvent
    WrTerminated,      // Waiting for thread termination.
    WrProcessInSwap,   // Waiting for a process to be swapped in.
    WrCpuRateControl,  // Waiting for CPU rate control.
    WrCalloutStack,    // Waiting for a callout stack.
    WrKernel,          // Waiting for a kernel event.
    WrResource,        // Waiting for a resource.
    WrPushLock,        // Waiting for a push lock.
    WrMutex,           // Waiting for a mutex.
    WrQuantumEnd,      // Waiting for the end of a quantum.           // 30
    WrDispatchInt,     // Waiting for a dispatch interrupt.
    WrPreempted,       // Waiting because the thread was preempted.
    WrYieldExecution,  // Waiting to yield execution.
    WrFastMutex,       // Waiting for a fast mutex.
    WrGuardedMutex,    // Waiting for a guarded mutex.
    WrRundown,         // Waiting for a rundown.
    WrAlertByThreadId, // Waiting for an alert by thread ID.
    WrDeferredPreempt, // Waiting for a deferred preemption.
    WrPhysicalFault,   // Waiting for a physical fault.
    WrIoRing,          // Waiting for an I/O ring.                    // 40
    WrMdlCache,        // Waiting for an MDL cache.
    WrRcu,             // Waiting for read-copy-update (RCU) synchronization.
    MaximumWaitReason
};
enum class KEY_VALUE_INFORMATION_CLASS_
{
    KeyValueBasicInformation,          // KEY_VALUE_BASIC_INFORMATION
    KeyValueFullInformation,           // KEY_VALUE_FULL_INFORMATION
    KeyValuePartialInformation,        // KEY_VALUE_PARTIAL_INFORMATION
    KeyValueFullInformationAlign64,    // KEY_VALUE_FULL_INFORMATION_ALIGN64
    KeyValuePartialInformationAlign64, // KEY_VALUE_PARTIAL_INFORMATION_ALIGN64
    KeyValueLayerInformation,          // KEY_VALUE_LAYER_INFORMATION
    MaxKeyValueInfoClass
};
enum class THREADINFOCLASS_
{
    ThreadBasicInformation,               // q: THREAD_BASIC_INFORMATION
    ThreadTimes,                          // q: KERNEL_USER_TIMES
    ThreadPriority,                       // s: KPRIORITY (requires SeIncreaseBasePriorityPrivilege)
    ThreadBasePriority,                   // s: KPRIORITY
    ThreadAffinityMask,                   // s: KAFFINITY
    ThreadImpersonationToken,             // s: HANDLE
    ThreadDescriptorTableEntry,           // q: DESCRIPTOR_TABLE_ENTRY (or WOW64_DESCRIPTOR_TABLE_ENTRY)
    ThreadEnableAlignmentFaultFixup,      // s: BOOLEAN
    ThreadEventPair,                      // Obsolete
    ThreadQuerySetWin32StartAddress,      // qs: PVOID (requires THREAD_Set_LIMITED_INFORMATION)
    ThreadZeroTlsCell,                    // s: ULONG // TlsIndex // 10
    ThreadPerformanceCount,               // q: LARGE_INTEGER
    ThreadAmILastThread,                  // q: ULONG
    ThreadIdealProcessor,                 // s: ULONG
    ThreadPriorityBoost,                  // qs: ULONG
    ThreadSetTlsArrayAddress,             // s: ULONG_PTR
    ThreadIsIoPending,                    // q: ULONG
    ThreadHideFromDebugger,               // q: BOOLEAN; s: void
    ThreadBreakOnTermination,             // qs: ULONG
    ThreadSwitchLegacyState,              // s: void // NtCurrentThread // NPX/FPU
    ThreadIsTerminated,                   // q: ULONG // 20
    ThreadLastSystemCall,                 // q: THREAD_LAST_SYSCALL_INFORMATION
    ThreadIoPriority,                     // qs: IO_PRIORITY_HINT (requires SeIncreaseBasePriorityPrivilege)
    ThreadCycleTime,                      // q: THREAD_CYCLE_TIME_INFORMATION (requires THREAD_QUERY_LIMITED_INFORMATION)
    ThreadPagePriority,                   // qs: PAGE_PRIORITY_INFORMATION
    ThreadActualBasePriority,             // s: LONG (requires SeIncreaseBasePriorityPrivilege)
    ThreadTebInformation,                 // q: THREAD_TEB_INFORMATION (requires THREAD_GET_CONTEXT + THREAD_SET_CONTEXT)
    ThreadCSwitchMon,                     // Obsolete
    ThreadCSwitchPmu,                     // Obsolete
    ThreadWow64Context,                   // qs: WOW64_CONTEXT, ARM_NT_CONTEXT since 20H1
    ThreadGroupInformation,               // qs: GROUP_AFFINITY // 30
    ThreadUmsInformation,                 // q: THREAD_UMS_INFORMATION // Obsolete
    ThreadCounterProfiling,               // q: BOOLEAN; s: THREAD_PROFILING_INFORMATION?
    ThreadIdealProcessorEx,               // qs: PROCESSOR_NUMBER; s: previous PROCESSOR_NUMBER on return
    ThreadCpuAccountingInformation,       // q: BOOLEAN; s: HANDLE (NtOpenSession) // NtCurrentThread // since WIN8
    ThreadSuspendCount,                   // q: ULONG // since WINBLUE
    ThreadHeterogeneousCpuPolicy,         // q: KHETERO_CPU_POLICY // since THRESHOLD
    ThreadContainerId,                    // q: GUID
    ThreadNameInformation,                // qs: THREAD_NAME_INFORMATION (requires THREAD_SET_LIMITED_INFORMATION)
    ThreadSelectedCpuSets,                // q: ULONG[]
    ThreadSystemThreadInformation,        // q: SYSTEM_THREAD_INFORMATION // 40
    ThreadActualGroupAffinity,            // q: GROUP_AFFINITY // since THRESHOLD2
    ThreadDynamicCodePolicyInfo,          // q: ULONG; s: ULONG (NtCurrentThread)
    ThreadExplicitCaseSensitivity,        // qs: ULONG; s: 0 disables, otherwise enables // (requires SeDebugPrivilege and PsProtectedSignerAntimalware)
    ThreadWorkOnBehalfTicket,             // ALPC_WORK_ON_BEHALF_TICKET // RTL_WORK_ON_BEHALF_TICKET_EX // NtCurrentThread
    ThreadSubsystemInformation,           // q: SUBSYSTEM_INFORMATION_TYPE // since REDSTONE2
    ThreadDbgkWerReportActive,            // s: ULONG; s: 0 disables, otherwise enables
    ThreadAttachContainer,                // s: HANDLE (job object) // NtCurrentThread
    ThreadManageWritesToExecutableMemory, // MANAGE_WRITES_TO_EXECUTABLE_MEMORY // since REDSTONE3
    ThreadPowerThrottlingState,           // qs: POWER_THROTTLING_THREAD_STATE // since REDSTONE3 (set), WIN11 22H2 (query)
    ThreadWorkloadClass,                  // THREAD_WORKLOAD_CLASS // since REDSTONE5 // 50
    ThreadCreateStateChange,              // since WIN11
    ThreadApplyStateChange,               // -
    ThreadStrongerBadHandleChecks,        // s: ULONG // NtCurrentThread // since 22H1
    ThreadEffectiveIoPriority,            // q: IO_PRIORITY_HINT
    ThreadEffectivePagePriority,          // q: ULONG
    ThreadUpdateLockOwnership,            // THREAD_LOCK_OWNERSHIP // since 24H2
    ThreadSchedulerSharedDataSlot,        // SCHEDULER_SHARED_DATA_SLOT_INFORMATION
    ThreadTebInformationAtomic,           // q: THREAD_TEB_INFORMATION (requires THREAD_GET_CONTEXT + THREAD_QUERY_INFORMATION)
    ThreadIndexInformation,               // THREAD_INDEX_INFORMATION
    MaxThreadInfoClass
};
enum class SECTION_INHERIT_
{
    ViewShare = 1,
    ViewUnmap = 2
};
enum class SHUTDOWN_ACTION_
{
    ShutdownNoReboot,
    ShutdownReboot,
    ShutdownPowerOff,
    ShutdownRebootForRecovery // since WIN11
};
enum class EVENT_TYPE_
{
    NotificationEvent,
    SynchronizationEvent
};

struct IO_STATUS_BLOCK_;
struct RTL_CRITICAL_SECTION_;
struct EXCEPTION_RECORD_;
struct ACTIVATION_CONTEXT_;
struct ACTIVATION_CONTEXT_DATA_;

using PPS_POST_PROCESS_INIT_ROUTINE_ = NT(NTAPI*)(NT);
using PIO_APC_ROUTINE_ = NT(NTAPI*)(GA ApcContext, IO_STATUS_BLOCK_* IoStatusBlock, U4 Reserved);
using PUSER_THREAD_START_ROUTINE_ = NTSTATUS(NTAPI*)(GA ThreadParameter);
using PLDR_INIT_ROUTINE_ = U1(NTAPI*)(GA DllHandle, U4 Reason, GA Context);
using PEXCEPTION_ROUTINE_ = EXCEPTION_DISPOSITION_(NTAPI*)(EXCEPTION_RECORD_* ExceptionRecord, GA EstablisherFrame, GA ContextRecord, GA DispatcherContext);
using PACTIVATION_CONTEXT_NOTIFY_ROUTINE_ = NT(NTAPI*)(U4 NotificationType, ACTIVATION_CONTEXT_* ActivationContext, ACTIVATION_CONTEXT_DATA_* ActivationContextData, GA NotificationContext,
                                                       GA NotificationData, U1* DisableThisNotification);

struct EXCEPTION_RECORD_
{
    U4 ExceptionCode;
    U4 ExceptionFlags;
    EXCEPTION_RECORD_* ExceptionRecord;
    GA ExceptionAddress;
    U4 NumberParameters;
    UA ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS];
};
struct EXCEPTION_REGISTRATION_RECORD_
{
    EXCEPTION_REGISTRATION_RECORD_* Next;
    PEXCEPTION_ROUTINE_ Handler;
};
struct RTL_ACTIVATION_CONTEXT_STACK_FRAME_
{
    RTL_ACTIVATION_CONTEXT_STACK_FRAME_* Previous;
    ACTIVATION_CONTEXT_* ActivationContext;
    U4 Flags;
};
struct TEB_ACTIVE_FRAME_CONTEXT_
{
    U4 Flags;
    cx CS* FrameName;
};
struct LARGE_INTEGER_
{
    union {
        struct
        {
            U4 LowPart;
            S4 HighPart;
        };
        S8 QuadPart;
    };
};
struct ULARGE_INTEGER_
{
    union {
        struct
        {
            U4 LowPart;
            U4 HighPart;
        };
        U8 QuadPart;
    };
};
struct ACL_
{
    U1 AclRevision;
    U1 Sbz1;
    U2 AclSize;
    U2 AceCount;
    U2 Sbz2;
};
struct SID_IDENTIFIER_AUTHORITY_
{
    U1 Value[6];
};
struct SID_
{
    U1 Revision;
    U1 SubAuthorityCount;
    SID_IDENTIFIER_AUTHORITY_ IdentifierAuthority;
    U4 SubAuthority flexarr;
};
struct UNICODE_STRING_
{
    U2 Length;
    U2 MaximumLength;
    CH* Buffer;

    dfa UNICODE_STRING_();
    dfa UNICODE_STRING_(cx CH* Buffer);
};
struct SECURITY_DESCRIPTOR_
{
    U1 Revision;
    U1 Sbz1;
    U2 Control;
    SID_* Owner;
    SID_* Group;
    ACL_* Sacl;
    ACL_* Dacl;
};
struct SECURITY_QUALITY_OF_SERVICE_
{
    U4 Length;
    SECURITY_IMPERSONATION_LEVEL_ ImpersonationLevel;
    U1 ContextTrackingMode;
    U1 EffectiveOnly;
};
struct OBJECT_ATTRIBUTES_
{
    U4 Length;
    HD RootDirectory;
    cx UNICODE_STRING_* ObjectName;
    U4 Attributes;
    SECURITY_DESCRIPTOR_* SecurityDescriptor;
    SECURITY_QUALITY_OF_SERVICE_* SecurityQualityOfService;

    dfa OBJECT_ATTRIBUTES_();
};
struct CLIENT_ID_
{
    HD UniqueProcess;
    HD UniqueThread;
};
struct STRING_
{
    U2 Length;
    U2 MaximumLength;
    CS* Buffer;
};
struct GUID_
{
    U4 Data1;
    U2 Data2;
    U2 Data3;
    U1 Data4[8];
};
struct SWITCH_CONTEXT_ATTRIBUTE_
{
    UA ContextUpdateCounter;
    S4 AllowContextUpdate;
    S4 EnableTrace;
    HD EtwHandle;
};
struct SWITCH_CONTEXT_DATA_
{
    U8 OsMaxVersionTested;
    U4 TargetPlatform;
    U8 ContextMinimum;
    GUID_ Platform;
    GUID_ MinPlatform;
    U4 ContextSource;
    U4 ElementCount;
    GUID_ Elements[48];
};
struct WER_FILE_
{
    U2 Flags;
    CH Path[MAX_PATH];
};
struct WER_MEMORY_
{
    GA Address;
    U4 Size;
};
struct LIST_ENTRY_
{
    LIST_ENTRY_* Flink;
    LIST_ENTRY_* Blink;
};
struct LDR_SERVICE_TAG_RECORD_
{
    LDR_SERVICE_TAG_RECORD_* Next;
    U4 ServiceTag;
};
struct SINGLE_LIST_ENTRY_
{
    SINGLE_LIST_ENTRY_* Next;
};
struct LDRP_CSLIST_
{
    SINGLE_LIST_ENTRY_* Tail;
};
struct ASSEMBLY_STORAGE_MAP_ENTRY_
{
    U4 Flags;
    UNICODE_STRING_ DosPath;
    HD Handle;
};
struct ASSEMBLY_STORAGE_MAP_
{
    U4 Flags;
    U4 AssemblyCount;
    ASSEMBLY_STORAGE_MAP_ENTRY_** AssemblyArray;
};
struct ACTIVATION_CONTEXT_
{
    S4 RefCount;
    U4 Flags;
    ACTIVATION_CONTEXT_DATA_* ActivationContextData;
    PACTIVATION_CONTEXT_NOTIFY_ROUTINE_ NotificationRoutine;
    GA NotificationContext;
    U4 SentNotifications[8];
    U4 DisabledNotifications[8];
    ASSEMBLY_STORAGE_MAP_ StorageMap;
    ASSEMBLY_STORAGE_MAP_ENTRY_* InlineStorageMapEntries[32];
};
struct LDR_DDAG_NODE_
{
    LIST_ENTRY_ Modules;
    LDR_SERVICE_TAG_RECORD_* ServiceTagList;
    U4 LoadCount;
    U4 LoadWhileUnloadingCount;
    U4 LowestLink;
    union {
        LDRP_CSLIST_ Dependencies;
        SINGLE_LIST_ENTRY_ RemovalLink;
    };
    LDRP_CSLIST_ IncomingDependencies;
    LDR_DDAG_STATE_ State;
    SINGLE_LIST_ENTRY_ CondenseLink;
    U4 PreorderNumber;
};
struct RTL_BALANCED_NODE_
{
    union {
        RTL_BALANCED_NODE_* Children[2];
        struct
        {
            RTL_BALANCED_NODE_* Left;
            RTL_BALANCED_NODE_* Right;
        };
    };
    union {
        U1 Red : 1;
        U1 Balance : 2;
        UA ParentValue;
    };
};
struct LDR_DATA_TABLE_ENTRY_
{
    LIST_ENTRY_ InLoadOrderLinks;
    LIST_ENTRY_ InMemoryOrderLinks;
    LIST_ENTRY_ InInitializationOrderLinks;
    GA DllBase;
    PLDR_INIT_ROUTINE_ EntryPoint;
    U4 SizeOfImage;
    UNICODE_STRING_ FullDllName;
    UNICODE_STRING_ BaseDllName;
    union {
        U1 FlagGroup[4];
        U4 Flags;
        struct
        {
            U4 PackagedBinary : 1;
            U4 MarkedForRemoval : 1;
            U4 ImageDll : 1;
            U4 LoadNotificationsSent : 1;
            U4 TelemetryEntryProcessed : 1;
            U4 ProcessStaticImport : 1;
            U4 InLegacyLists : 1;
            U4 InIndexes : 1;
            U4 ShimDll : 1;
            U4 InExceptionTable : 1;
            U4 ReservedFlags1 : 2;
            U4 LoadInProgress : 1;
            U4 LoadConfigProcessed : 1;
            U4 EntryProcessed : 1;
            U4 ProtectDelayLoad : 1;
            U4 ReservedFlags3 : 2;
            U4 DontCallForThreads : 1;
            U4 ProcessAttachCalled : 1;
            U4 ProcessAttachFailed : 1;
            U4 CorDeferredValidate : 1;
            U4 CorImage : 1;
            U4 DontRelocate : 1;
            U4 CorILOnly : 1;
            U4 ChpeImage : 1;
            U4 ChpeEmulatorImage : 1;
            U4 ReservedFlags5 : 1;
            U4 Redirected : 1;
            U4 ReservedFlags6 : 2;
            U4 CompatDatabaseProcessed : 1;
        };
    };
    U2 ObsoleteLoadCount;
    U2 TlsIndex;
    LIST_ENTRY_ HashLinks;
    U4 TimeDateStamp;
    ACTIVATION_CONTEXT_* EntryPointActivationContext;
    GA Lock;
    LDR_DDAG_NODE_* DdagNode;
    LIST_ENTRY_ NodeModuleLink;
    GA LoadContext;
    GA ParentDllBase;
    GA SwitchBackContext;
    RTL_BALANCED_NODE_ BaseAddressIndexNode;
    RTL_BALANCED_NODE_ MappingInfoIndexNode;
    GA OriginalBase;
    LARGE_INTEGER_ LoadTime;
    U4 BaseNameHashValue;
    LDR_DLL_LOAD_REASON_ LoadReason;
    U4 ImplicitPathOptions;
    U4 ReferenceCount;
    U4 DependentLoadFlags;
    U1 SigningLevel;
    U4 CheckSum;
    GA ActivePatchImageBase;
    LDR_HOT_PATCH_STATE_ HotPatchState;
};
struct CURDIR_
{
    UNICODE_STRING_ DosPath;
    HD Handle;
};
struct RTL_DRIVE_LETTER_CURDIR_
{
    U2 Flags;
    U2 Length;
    U4 TimeStamp;
    STRING_ DosPath;
};
struct RTL_CRITICAL_SECTION_DEBUG_
{
    U2 Type;
    U2 CreatorBackTraceIndex;
    RTL_CRITICAL_SECTION_* CriticalSection;
    LIST_ENTRY_ ProcessLocksList;
    U4 EntryCount;
    U4 ContentionCount;
    U4 Flags;
    U2 CreatorBackTraceIndexHigh;
    U2 SpareU2;
};
struct SLIST_ENTRY_
{
    SLIST_ENTRY_* Next;
};
struct KSYSTEM_TIME_
{
    U4 LowPart;
    S4 High1Time;
    S4 High2Time;
};
struct SDBQUERYRESULT_
{
    U4 Exes[16];
    U4 ExeFlags[16];
    U4 Layers[8];
    U4 LayerFlags;
    U4 AppHelp;
    U4 ExeCount;
    U4 LayerCount;
    GUID_ ID;
    U4 ExtraFlags;
    U4 CustomSDBMap;
    GUID_ DB[16];
};
struct SWITCH_CONTEXT_
{
    SWITCH_CONTEXT_ATTRIBUTE_ Attribute;
    SWITCH_CONTEXT_DATA_ Data;
};
struct SDB_CSTRUCT_COBALT_PROCFLAG_
{
    UA AffinityMask;
    U4 CPUIDEcxOverride;
    U4 CPUIDEdxOverride;
    U2 ProcessorGroup;
    U2 FastSelfModThreshold;
    U2 Reserved1;
    U1 Reserved2;
    U1 BackgroundWork : 5;
    U1 CPUIDBrand : 4;
    U1 Reserved3 : 4;
    U1 RdtscScaling : 3;
    U1 Reserved4 : 2;
    U1 UnalignedAtomicApproach : 2;
    U1 Win11Atomics : 2;
    U1 RunOnSingleCore : 1;
    U1 X64CPUID : 1;
    U1 PatchUnaligned : 1;
    U1 InterpreterOrJitter : 1;
    U1 ForceSegmentHeap : 1;
    U1 Reserved5 : 1;
    U1 Reserved6 : 1;
    union {
        U8 Group1AsUINT64;
        SDB_CSTRUCT_COBALT_PROCFLAG_* Specified;
    };
};
struct WER_RECOVERY_INFO_
{
    U4 Length;
    GA Callback;
    GA Parameter;
    HD Started;
    HD Finished;
    HD InProgress;
    S4 LastError;
    S4 Successful;
    U4 PingInterval;
    U4 Flags;
};
struct WER_GATHER_
{
    GA Next;
    U2 Flags;
    union {
        WER_FILE_ File;
        WER_MEMORY_ Memory;
    } v;
};
struct WER_METADATA_
{
    GA Next;
    CH Key[64];
    CH Value[128];
};
struct WER_RUNTIME_DLL_
{
    GA Next;
    U4 Length;
    GA Context;
    CH CallbackDllPath[MAX_PATH];
};
struct WER_DUMP_COLLECTION_
{
    GA Next;
    U4 ProcessId;
    U4 ThreadId;
};
struct WER_HEAP_MAIN_HEADER_
{
    CH Signature[16];
    LIST_ENTRY_ Links;
    HD Mutex;
    GA FreeHeap;
    U4 FreeCount;
};
struct PEB_LDR_DATA_
{
    U4 Length;
    U1 Initialized;
    HD SsHandle;
    LIST_ENTRY_ InLoadOrderModuleList;
    LIST_ENTRY_ InMemoryOrderModuleList;
    LIST_ENTRY_ InInitializationOrderModuleList;
    GA EntryInProgress;
    U1 ShutdownInProgress;
    HD ShutdownThreadId;
};
struct RTL_USER_PROCESS_PARAMETERS_
{
    U4 MaximumLength;
    U4 Length;
    U4 Flags;
    U4 DebugFlags;
    HD ConsoleHandle;
    U4 ConsoleFlags;
    HD StandardInput;
    HD StandardOutput;
    HD StandardError;
    CURDIR_ CurrentDirectory;
    UNICODE_STRING_ DllPath;
    UNICODE_STRING_ ImagePathName;
    UNICODE_STRING_ CommandLine;
    GA Environment;
    U4 StartingX;
    U4 StartingY;
    U4 CountX;
    U4 CountY;
    U4 CountCharsX;
    U4 CountCharsY;
    U4 FillAttribute;
    U4 WindowFlags;
    U4 ShowWindowFlags;
    UNICODE_STRING_ WindowTitle;
    UNICODE_STRING_ DesktopInfo;
    UNICODE_STRING_ ShellInfo;
    UNICODE_STRING_ RuntimeData;
    RTL_DRIVE_LETTER_CURDIR_ CurrentDirectories[RTL_MAX_DRIVE_LETTERS];
    UA EnvironmentSize;
    UA EnvironmentVersion;
    GA PackageDependencyData;
    U4 ProcessGroupId;
    U4 LoaderThreads;
    UNICODE_STRING_ RedirectionDllName;
    UNICODE_STRING_ HeapPartitionName;
    U8* DefaultThreadpoolCpuSetMasks;
    U4 DefaultThreadpoolCpuSetMaskCount;
    U4 DefaultThreadpoolThreadMaximum;
    U4 HeapMemoryTypeMask;
};
struct RTL_CRITICAL_SECTION_
{
    RTL_CRITICAL_SECTION_DEBUG_* DebugInfo;
    S4 LockCount;
    S4 RecursionCount;
    HD OwningThread;
    HD LockSemaphore;
    UA SpinCount;
};
struct SLIST_HEADER_
{
    union {
        U8 Alignment;
        struct
        {
            SLIST_ENTRY_ Next;
            U2 Depth;
            U2 Sequence;
        };
    };
};
struct API_SET_NAMESPACE_
{
    U4 Version;
    U4 Size;
    U4 Flags;
    U4 Count;
    U4 EntryOffset;
    U4 HashOffset;
    U4 HashFactor;
};
struct RTL_BITMAP_
{
    U4 SizeOfBitMap;
    U4* Buffer;
};
struct SILO_USER_SHARED_DATA_
{
    U4 ServiceSessionId;
    U4 ActiveConsoleId;
    S8 ConsoleSessionForegroundProcessId;
    NT_PRODUCT_TYPE_ NtProductType;
    U4 SuiteMask;
    U4 SharedUserSessionId;
    U1 IsMultiSessionSku;
    U1 IsStateSeparationEnabled;
    CH NtSystemRoot[260];
    U2 UserModeGlobalLogger[16];
    U4 TimeZoneId;
    S4 TimeZoneBiasStamp;
    KSYSTEM_TIME_ TimeZoneBias;
    LARGE_INTEGER_ TimeZoneBiasEffectiveStart;
    LARGE_INTEGER_ TimeZoneBiasEffectiveEnd;
};
struct GDI_HANDLE_BUFFER_
{
    UA Data1[26];
    U4 Data2[8];
};
struct APPCOMPAT_EXE_DATA_
{
    UA Reserved[65];
    U4 Size;
    U4 Magic;
    S4 LoadShimEngine;
    U2 ExeType;
    SDBQUERYRESULT_ SdbQueryResult;
    UA DbgLogChannels[128];
    SWITCH_CONTEXT_ SwitchContext;
    U4 ParentProcessId;
    CH ParentImageName[260];
    CH ParentCompatLayers[256];
    CH ActiveCompatLayers[256];
    U4 ImageFileSize;
    U4 ImageCheckSum;
    S4 LatestOs;
    S4 PackageId;
    S4 SwitchBackManifest;
    S4 UacManifest;
    S4 LegacyInstaller;
    U4 RunLevel;
    UA WinRTFlags;
    GA HookCOM;
    GA ComponentOnDemandEvent;
    GA Quirks;
    U4 QuirksSize;
    SDB_CSTRUCT_COBALT_PROCFLAG_ CobaltProcFlags;
    U4 FullMatchDbSizeCb;
    U4 FullMatchDbOffset;
};
struct ACTIVATION_CONTEXT_DATA_
{
    U4 Magic;
    U4 HeaderSize;
    U4 FormatVersion;
    U4 TotalSize;
    U4 DefaultTocOffset;
    U4 ExtendedTocOffset;
    U4 AssemblyRosterOffset;
    U4 Flags;
};
struct WER_PEB_HEADER_BLOCK_
{
    S4 Length;
    CH Signature[16];
    CH AppDataRelativePath[64];
    CH RestartCommandLine[RESTART_MAX_CMD_LINE];
    WER_RECOVERY_INFO_ RecoveryInfo;
    WER_GATHER_* Gather;
    WER_METADATA_* MetaData;
    WER_RUNTIME_DLL_* RuntimeDll;
    WER_DUMP_COLLECTION_* DumpCollection;
    S4 GatherCount;
    S4 MetaDataCount;
    S4 DumpCount;
    S4 Flags;
    WER_HEAP_MAIN_HEADER_ MainHeader;
    GA Reserved;
};
struct TELEMETRY_COVERAGE_HEADER_
{
    U1 MajorVersion;
    U1 MinorVersion;
    struct
    {
        U2 TracingEnabled : 1;
        U2 Reserved1 : 15;
    };
    U4 HashTableEntries;
    U4 HashIndexMask;
    U4 TableUpdateVersion;
    U4 TableSizeInBytes;
    U4 LastResetTick;
    U4 ResetRound;
    U4 Reserved2;
    U4 RecordedCount;
    U4 Reserved3[4];
    U4 HashTable flexarr;
};
struct LEAP_SECOND_DATA_
{
    U1 Enabled;
    U1 Padding[3];
    U4 Count;
    LARGE_INTEGER_ Data flexarr;
};
struct NT_TIB_
{
    EXCEPTION_REGISTRATION_RECORD_* ExceptionList;
    GA StackBase;
    GA StackLimit;
    GA SubSystemTib;
    union {
        GA FiberData;
        U4 Version;
    };
    GA ArbitraryUserPointer;
    NT_TIB_* Self;
};
struct ACTIVATION_CONTEXT_STACK_
{
    RTL_ACTIVATION_CONTEXT_STACK_FRAME_* ActiveFrame;
    LIST_ENTRY_ FrameListCache;
    U4 Flags;
    U4 NextCookieSequenceNumber;
    U4 StackId;
};
struct GDI_TEB_BATCH_
{
    U4 Offset;
    UA HDC;
    U4 Buffer[GDI_BATCH_BUFFER_SIZE];
};
struct PROCESSOR_NUMBER_
{
    U2 Group;
    U1 Number;
    U1 Reserved;
};
struct SOleTlsData_
{
    GA ThreadBase;
    GA SmAllocator;
    U4 ApartmentID;
    U4 Flags;
    S4 TlsMapIndex;
    GA* TlsSlot;
    U4 ComInits;
    U4 OleInits;
    U4 Calls;
    GA ServerCall;
    GA CallObjectCache;
    GA ContextStack;
    GA ObjServer;
    U4 TIDCaller;
    U1 VersionDependant flexarr;
};
struct TEB_ACTIVE_FRAME_
{
    U4 Flags;
    TEB_ACTIVE_FRAME_* Previous;
    TEB_ACTIVE_FRAME_CONTEXT_* Context;
};
struct GROUP_AFFINITY_
{
    UA Mask;
    U2 Group;
    U2 Reserved[3];
};
struct PEB_
{
    U1 InheritedAddressSpace;
    U1 ReadImageFileExecOptions;
    U1 BeingDebugged;
    union {
        U1 BitField;
        struct
        {
            U1 ImageUsesLargePages : 1;
            U1 IsProtectedProcess : 1;
            U1 IsImageDynamicallyRelocated : 1;
            U1 SkipPatchingUser32Forwarders : 1;
            U1 IsPackagedProcess : 1;
            U1 IsAppContainer : 1;
            U1 IsProtectedProcessLight : 1;
            U1 IsLongPathAwareProcess : 1;
        };
    };
    HD Mutant;
    GA ImageBaseAddress;
    PEB_LDR_DATA_* Ldr;
    RTL_USER_PROCESS_PARAMETERS_* ProcessParameters;
    GA SubSystemData;
    GA ProcessHeap;
    RTL_CRITICAL_SECTION_* FastPebLock;
    SLIST_HEADER_* AtlThunkSListPtr;
    GA IFEOKey;
    union {
        U4 CrossProcessFlags;
        struct
        {
            U4 ProcessInJob : 1;
            U4 ProcessInitializing : 1;
            U4 ProcessUsingVEH : 1;
            U4 ProcessUsingVCH : 1;
            U4 ProcessUsingFTH : 1;
            U4 ProcessPreviouslyThrottled : 1;
            U4 ProcessCurrentlyThrottled : 1;
            U4 ProcessImagesHotPatched : 1;
            U4 ReservedBits0 : 24;
        };
    };
    union {
        GA KernelCallbackTable;
        GA UserSharedInfoPtr;
    };
    U4 SystemReserved;
    U4 AtlThunkSListPtr32;
    API_SET_NAMESPACE_* ApiSetMap;
    U4 TlsExpansionCounter;
    RTL_BITMAP_* TlsBitmap;
    U4 TlsBitmapBits[2];
    GA ReadOnlySharedMemoryBase;
    SILO_USER_SHARED_DATA_* SharedData;
    GA* ReadOnlyStaticServerData;
    GA AnsiCodePageData;
    GA OemCodePageData;
    GA UnicodeCaseTableData;
    U4 NumberOfProcessors;
    union {
        U4 NtGlobalFlag;
        struct
        {
            U4 StopOnException : 1;          // FLG_STOP_ON_EXCEPTION
            U4 ShowLoaderSnaps : 1;          // FLG_SHOW_LDR_SNAPS
            U4 DebugInitialCommand : 1;      // FLG_DEBUG_INITIAL_COMMAND
            U4 StopOnHungGUI : 1;            // FLG_STOP_ON_HUNG_GUI
            U4 HeapEnableTailCheck : 1;      // FLG_HEAP_ENABLE_TAIL_CHECK
            U4 HeapEnableFreeCheck : 1;      // FLG_HEAP_ENABLE_FREE_CHECK
            U4 HeapValidateParameters : 1;   // FLG_HEAP_VALIDATE_PARAMETERS
            U4 HeapValidateAll : 1;          // FLG_HEAP_VALIDATE_ALL
            U4 ApplicationVerifier : 1;      // FLG_APPLICATION_VERIFIER
            U4 MonitorSilentProcessExit : 1; // FLG_MONITOR_SILENT_PROCESS_EXIT
            U4 PoolEnableTagging : 1;        // FLG_POOL_ENABLE_TAGGING
            U4 HeapEnableTagging : 1;        // FLG_HEAP_ENABLE_TAGGING
            U4 UserStackTraceDb : 1;         // FLG_USER_STACK_TRACE_DB
            U4 KernelStackTraceDb : 1;       // FLG_KERNEL_STACK_TRACE_DB
            U4 MaintainObjectTypeList : 1;   // FLG_MAINTAIN_OBJECT_TYPELIST
            U4 HeapEnableTagByDll : 1;       // FLG_HEAP_ENABLE_TAG_BY_DLL
            U4 DisableStackExtension : 1;    // FLG_DISABLE_STACK_EXTENSION
            U4 EnableCsrDebug : 1;           // FLG_ENABLE_CSRDEBUG
            U4 EnableKDebugSymbolLoad : 1;   // FLG_ENABLE_KDEBUG_SYMBOL_LOAD
            U4 DisablePageKernelStacks : 1;  // FLG_DISABLE_PAGE_KERNEL_STACKS
            U4 EnableSystemCritBreaks : 1;   // FLG_ENABLE_SYSTEM_CRIT_BREAKS
            U4 HeapDisableCoalescing : 1;    // FLG_HEAP_DISABLE_COALESCING
            U4 EnableCloseExceptions : 1;    // FLG_ENABLE_CLOSE_EXCEPTIONS
            U4 EnableExceptionLogging : 1;   // FLG_ENABLE_EXCEPTION_LOGGING
            U4 EnableHandleTypeTagging : 1;  // FLG_ENABLE_HANDLE_TYPE_TAGGING
            U4 HeapPageAllocs : 1;           // FLG_HEAP_PAGE_ALLOCS
            U4 DebugInitialCommandEx : 1;    // FLG_DEBUG_INITIAL_COMMAND_EX
            U4 DisableDbgPrint : 1;          // FLG_DISABLE_DBGPRINT
            U4 CritSecEventCreation : 1;     // FLG_CRITSEC_EVENT_CREATION
            U4 LdrTopDown : 1;               // FLG_LDR_TOP_DOWN
            U4 EnableHandleExceptions : 1;   // FLG_ENABLE_HANDLE_EXCEPTIONS
            U4 DisableProtDlls : 1;          // FLG_DISABLE_PROTDLLS
        } NtGlobalFlags;
    };
    LARGE_INTEGER_ CriticalSectionTimeout;
    SI HeapSegmentReserve;
    SI HeapSegmentCommit;
    SI HeapDeCommitTotalFreeThreshold;
    SI HeapDeCommitFreeBlockThreshold;
    U4 NumberOfHeaps;
    U4 MaximumNumberOfHeaps;
    GA* ProcessHeaps;
    GA GdiSharedHandleTable;
    GA ProcessStarterHelper;
    U4 GdiDCAttributeList;
    RTL_CRITICAL_SECTION_* LoaderLock;
    U4 OSMajorVersion;
    U4 OSMinorVersion;
    U2 OSBuildNumber;
    U2 OSCSDVersion;
    U4 OSPlatformId;
    U4 ImageSubsystem;
    U4 ImageSubsystemMajorVersion;
    U4 ImageSubsystemMinorVersion;
    UA ActiveProcessAffinityMask;
    GDI_HANDLE_BUFFER_ GdiHandleBuffer;
    PPS_POST_PROCESS_INIT_ROUTINE_ PostProcessInitRoutine;
    RTL_BITMAP_* TlsExpansionBitmap;
    U4 TlsExpansionBitmapBits[32];
    U4 SessionId;
    ULARGE_INTEGER_ AppCompatFlags;
    ULARGE_INTEGER_ AppCompatFlagsUser;
    GA pShimData;
    APPCOMPAT_EXE_DATA_* AppCompatInfo;
    UNICODE_STRING_ CSDVersion;
    ACTIVATION_CONTEXT_DATA_* ActivationContextData;
    ASSEMBLY_STORAGE_MAP_* ProcessAssemblyStorageMap;
    ACTIVATION_CONTEXT_DATA_* SystemDefaultActivationContextData;
    ASSEMBLY_STORAGE_MAP_* SystemAssemblyStorageMap;
    SI MinimumStackCommit;
    GA SparePointers[2];
    GA PatchLoaderData;
    GA ChpeV2ProcessInfo;
    U4 AppModelFeatureState;
    U4 SpareUlongs[2];
    U2 ActiveCodePage;
    U2 OemCodePage;
    U2 UseCaseMapping;
    U2 UnusedNlsField;
    WER_PEB_HEADER_BLOCK_* WerRegistrationData;
    GA WerShipAssertPtr;
    union {
        GA pContextData;
        GA EcCodeBitMap;
    };
    GA pImageHeaderHash;
    union {
        U4 TracingFlags;
        struct
        {
            U4 HeapTracingEnabled : 1;
            U4 CritSecTracingEnabled : 1;
            U4 LibLoaderTracingEnabled : 1;
            U4 SpareTracingBits : 29;
        };
    };
    U8 CsrServerReadOnlySharedMemoryBase;
    RTL_CRITICAL_SECTION_* TppWorkerpListLock;
    LIST_ENTRY_ TppWorkerpList;
    GA WaitOnAddressHashTable[128];
    TELEMETRY_COVERAGE_HEADER_* TelemetryCoverageHeader;
    U4 CloudFileFlags;
    U4 CloudFileDiagFlags;
    S1 PlaceholderCompatibilityMode;
    S1 PlaceholderCompatibilityModeReserved[7];
    LEAP_SECOND_DATA_* LeapSecondData;
    union {
        U4 LeapSecondFlags;
        struct
        {
            U4 SixtySecondEnabled : 1;
            U4 Reserved : 31;
        };
    };
    U4 NtGlobalFlag2;
    U8 ExtendedFeatureDisableMask;
};
struct TEB_
{
    NT_TIB_ NtTib;
    GA EnvironmentPointer;
    CLIENT_ID_ ClientId;
    GA ActiveRpcHandle;
    GA ThreadLocalStoragePointer;
    PEB_* ProcessEnvironmentBlock;
    U4 LastErrorValue;
    U4 CountOfOwnedCriticalSections;
    GA CsrClientThread;
    GA Win32ThreadInfo;
    U4 User32Reserved[26];
    U4 UserReserved[5];
    GA WOW32Reserved;
    U4 CurrentLocale;
    U4 FpSoftwareStatusRegister;
    GA ReservedForDebuggerInstrumentation[16];
#ifdef PROG_ADR_SIZE_8
    GA SystemReserved1[25];
    GA HeapFlsData;
    UA RngState[4];
#else
    GA SystemReserved1[26];
#endif
    S1 PlaceholderCompatibilityMode;
    U1 PlaceholderHydrationAlwaysExplicit;
    S1 PlaceholderReserved[10];
    U4 ProxiedProcessId;
    ACTIVATION_CONTEXT_STACK_ ActivationStack;
    U1 WorkingOnBehalfTicket[8];
    NTSTATUS ExceptionCode;
    ACTIVATION_CONTEXT_STACK_* ActivationContextStackPointer;
    UA InstrumentationCallbackSp;
    UA InstrumentationCallbackPreviousPc;
    UA InstrumentationCallbackPreviousSp;
#ifdef PROG_ADR_SIZE_8
    U4 TxFsContext;
#endif
    U1 InstrumentationCallbackDisabled;
#ifdef PROG_ADR_SIZE_8
    U1 UnalignedLoadStoreExceptions;
#endif
#ifdef PROG_ADR_SIZE_4
    U1 SpareBytes[23];
    U4 TxFsContext;
#endif
    GDI_TEB_BATCH_ GdiTebBatch;
    CLIENT_ID_ RealClientId;
    HD GdiCachedProcessHandle;
    U4 GdiClientPID;
    U4 GdiClientTID;
    GA GdiThreadLocalInfo;
    UA Win32ClientInfo[WIN32_CLIENT_INFO_LENGTH];
    GA glDispatchTable[233];
    UA glReserved1[29];
    GA glReserved2;
    GA glSectionInfo;
    GA glSection;
    GA glTable;
    GA glCurrentRC;
    GA glContext;
    NTSTATUS LastStatusValue;
    UNICODE_STRING_ StaticUnicodeString;
    CH StaticUnicodeBuffer[STATIC_UNICODE_BUFFER_LENGTH];
    GA DeallocationStack;
    GA TlsSlots[TLS_MINIMUM_AVAILABLE];
    LIST_ENTRY_ TlsLinks;
    GA Vdm;
    GA ReservedForNtRpc;
    GA DbgSsReserved[2];
    U4 HardErrorMode;
#ifdef PROG_ADR_SIZE_8
    GA Instrumentation[11];
#else
    GA Instrumentation[9];
#endif
    GUID_ ActivityId;
    GA SubProcessTag;
    GA PerflibData;
    GA EtwTraceData;
    HD WinSockData;
    U4 GdiBatchCount;
    union {
        PROCESSOR_NUMBER_ CurrentIdealProcessor;
        U4 IdealProcessorValue;
        struct
        {
            U1 ReservedPad0;
            U1 ReservedPad1;
            U1 ReservedPad2;
            U1 IdealProcessor;
        };
    };
    U4 GuaranteedStackBytes;
    GA ReservedForPerf;
    SOleTlsData_* ReservedForOle;
    U4 WaitingOnLoaderLock;
    GA SavedPriorityState;
    UA ReservedForCodeCoverage;
    GA ThreadPoolData;
    GA* TlsExpansionSlots;
#ifdef PROG_ADR_SIZE_8
    GA ChpeV2CpuAreaInfo;
    GA Unused;
#endif
    U4 MuiGeneration;
    U4 IsImpersonating;
    GA NlsCache;
    GA pShimData;
    U4 HeapData;
    HD CurrentTransactionHandle;
    TEB_ACTIVE_FRAME_* ActiveFrame;
    GA FlsData;
    GA PreferredLanguages;
    GA UserPrefLanguages;
    GA MergedPrefLanguages;
    U4 MuiImpersonation;
    union {
        U2 CrossTebFlags;
        U2 SpareCrossTebBits : 16;
    };
    union {
        U2 SameTebFlags;
        struct
        {
            U2 SafeThunkCall : 1;
            U2 InDebugPrint : 1;
            U2 HasFiberData : 1;
            U2 SkipThreadAttach : 1;
            U2 WerInShipAssertCode : 1;
            U2 RanProcessInit : 1;
            U2 ClonedThread : 1;
            U2 SuppressDebugMsg : 1;
            U2 DisableUserStackWalk : 1;
            U2 RtlExceptionAttached : 1;
            U2 InitialThread : 1;
            U2 SessionAware : 1;
            U2 LoadOwner : 1;
            U2 LoaderWorker : 1;
            U2 SkipLoaderInit : 1;
            U2 SkipFileAPIBrokering : 1;
        };
    };
    GA TxnScopeEnterCallback;
    GA TxnScopeExitCallback;
    GA TxnScopeContext;
    U4 LockCount;
    S4 WowTebOffset;
    GA ResourceRetValue;
    GA ReservedForWdf;
    U8 ReservedForCrt;
    GUID_ EffectiveContainerId;
    U8 LastSleepCounter;
    U4 SpinCallCount;
    U8 ExtendedFeatureDisableMask;
    GA SchedulerSharedDataSlot;
    GA HeapWalkContext;
    GROUP_AFFINITY_ PrimaryGroupAffinity;
    U4 Rcu[2];
};
struct PROCESS_BASIC_INFORMATION_
{
    NTSTATUS ExitStatus;
    PEB_* PebBaseAddress;
    UA AffinityMask;
    S4 BasePriority;
    HD UniqueProcessId;
    HD InheritedFromUniqueProcessId;
};
struct IMAGE_DATA_DIRECTORY_
{
    U4 VirtualAddress;
    U4 Size;
};
struct IMAGE_DOS_HEADER_
{
    U2 e_magic;
    U2 e_cblp;
    U2 e_cp;
    U2 e_crlc;
    U2 e_cparhdr;
    U2 e_minalloc;
    U2 e_maxalloc;
    U2 e_ss;
    U2 e_sp;
    U2 e_csum;
    U2 e_ip;
    U2 e_cs;
    U2 e_lfarlc;
    U2 e_ovno;
    U2 e_res[4];
    U2 e_oemid;
    U2 e_oeminfo;
    U2 e_res2[10];
    S4 e_lfanew;
};
struct IMAGE_FILE_HEADER_
{
    U2 Machine;
    U2 NumberOfSections;
    U4 TimeDateStamp;
    U4 PointerToSymbolTable;
    U4 NumberOfSymbols;
    U2 SizeOfOptionalHeader;
    U2 Characteristics;
};
struct IMAGE_OPTIONAL_HEADER_
{
    U2 Magic;
    U1 MajorLinkerVersion;
    U1 MinorLinkerVersion;
    U4 SizeOfCode;
    U4 SizeOfInitializedData;
    U4 SizeOfUninitializedData;
    U4 AddressOfEntryPoint;
    U4 BaseOfCode;
#ifdef PROG_ADR_SIZE_4
    U4 BaseOfData;
    U4 ImageBase;
#endif
#ifdef PROG_ADR_SIZE_8
    U8 ImageBase;
#endif
    U4 SectionAlignment;
    U4 FileAlignment;
    U2 MajorOperatingSystemVersion;
    U2 MinorOperatingSystemVersion;
    U2 MajorImageVersion;
    U2 MinorImageVersion;
    U2 MajorSubsystemVersion;
    U2 MinorSubsystemVersion;
    U4 Win32VersionValue;
    U4 SizeOfImage;
    U4 SizeOfHeaders;
    U4 CheckSum;
    U2 Subsystem;
    U2 DllCharacteristics;
    UA SizeOfStackReserve;
    UA SizeOfStackCommit;
    UA SizeOfHeapReserve;
    UA SizeOfHeapCommit;
    U4 LoaderFlags;
    U4 NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY_ DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
};
struct IMAGE_NT_HEADERS_
{
    U4 Signature;
    IMAGE_FILE_HEADER_ FileHeader;
    IMAGE_OPTIONAL_HEADER_ OptionalHeader;
};
struct IMAGE_EXPORT_DIRECTORY_
{
    U4 Characteristics;
    U4 TimeDateStamp;
    U2 MajorVersion;
    U2 MinorVersion;
    U4 Name;
    U4 Base;
    U4 NumberOfFunctions;
    U4 NumberOfNames;
    U4 AddressOfFunctions;
    U4 AddressOfNames;
    U4 AddressOfNameOrdinals;
};
struct LUID_
{
    U4 LowPart;
    S4 HighPart;
};
struct LUID_AND_ATTRIBUTES_
{
    LUID_ Luid;
    U4 Attributes;
};
struct PS_ATTRIBUTE_
{
    UA Attribute;
    UA Size;
    union {
        UA Value;
        GA ValuePtr;
    };
    UA* ReturnLength;
};
struct TOKEN_PRIVILEGES_
{
    U4 PrivilegeCount;
    LUID_AND_ATTRIBUTES_ Privileges flexarr;
};
struct SID_AND_ATTRIBUTES_
{
    SID_* Sid;
    U4 Attributes;
};
struct TOKEN_USER_
{
    SID_AND_ATTRIBUTES_ User;
};
struct PS_ATTRIBUTE_LIST_
{
    UA TotalLength;
    PS_ATTRIBUTE_ Attributes flexarr;
};
struct PS_CREATE_INFO_
{
    UA Size;
    PS_CREATE_STATE_ State;
    union {
        struct // PsCreateInitialState
        {
            union {
                U4 InitFlags;
                struct
                {
                    U1 WriteOutputOnExit : 1;
                    U1 DetectManifest : 1;
                    U1 IFEOSkipDebugger : 1;
                    U1 IFEODoNotPropagateKeyState : 1;
                    U1 SpareBits1 : 4;
                    U1 SpareBits2 : 8;
                    U2 ProhibitedImageCharacteristics : 16;
                };
            };
            U4 AdditionalFileAccess;
        } InitState;
        struct // PsCreateFailOnSectionCreate
        {
            HD FileHandle;
        } FailSection;
        struct // PsCreateFailExeFormat
        {
            U2 DllCharacteristics;
        } ExeFormat;
        struct // PsCreateFailExeName
        {
            HD IFEOKey;
        } ExeName;
        struct // PsCreateSuccess
        {
            union {
                U4 OutputFlags;
                struct
                {
                    U1 ProtectedProcess : 1;
                    U1 AddressSpaceOverride : 1;
                    U1 DevOverrideEnabled : 1; // from Image File Execution Options
                    U1 ManifestDetected : 1;
                    U1 ProtectedProcessLight : 1;
                    U1 SpareBits1 : 3;
                    U1 SpareBits2 : 8;
                    U2 SpareBits3 : 16;
                };
            };
            HD FileHandle;
            HD SectionHandle;
            U8 UserProcessParametersNative;
            U4 UserProcessParametersWow64;
            U4 CurrentParameterFlags;
            U8 PebAddressNative;
            U4 PebAddressWow64;
            U8 ManifestAddress;
            U4 ManifestSize;
        } SuccessState;
    };
};
struct SYSTEM_THREAD_INFORMATION_
{
    LARGE_INTEGER_ KernelTime;  // Number of 100-nanosecond intervals spent executing kernel code.
    LARGE_INTEGER_ UserTime;    // Number of 100-nanosecond intervals spent executing user code.
    LARGE_INTEGER_ CreateTime;  // The date and time when the thread was created.
    U4 WaitTime;                // The current time spent in ready queue or waiting (depending on the thread state).
    GA StartAddress;            // The initial start address of the thread.
    CLIENT_ID_ ClientId;        // The identifier of the thread and the process owning the thread.
    S4 Priority;                // The dynamic priority of the thread.
    S4 BasePriority;            // The starting priority of the thread.
    U4 ContextSwitches;         // The total number of context switches performed.
    KTHREAD_STATE_ ThreadState; // The current state of the thread.
    KWAIT_REASON_ WaitReason;   // The current reason the thread is waiting.
};
struct SYSTEM_PROCESS_INFORMATION_
{
    U4 NextEntryOffset;                 // The address of the previous item plus the value in the NextEntryOffset member. For the last item in the array, NextEntryOffset is 0.
    U4 NumberOfThreads;                 // The NumberOfThreads member contains the number of threads in the process.
    U8 WorkingSetPrivateSize;           // The total private memory that a process currently has allocated and is physically resident in memory. // since VISTA
    U4 HardFaultCount;                  // The total number of hard faults for data from disk rather than from in-memory pages. // since WIN7
    U4 NumberOfThreadsHighWatermark;    // The peak number of threads that were running at any given point in time, indicative of potential performance bottlenecks related to thread management.
    U8 CycleTime;                       // The sum of the cycle time of all threads in the process.
    LARGE_INTEGER_ CreateTime;          // Number of 100-nanosecond intervals since the creation time of the process. Not updated during system timezone changes.
    LARGE_INTEGER_ UserTime;            // Number of 100-nanosecond intervals the process has executed in user mode.
    LARGE_INTEGER_ KernelTime;          // Number of 100-nanosecond intervals the process has executed in kernel mode.
    UNICODE_STRING_ ImageName;          // The file name of the executable image.
    S4 BasePriority;                    // The starting priority of the process.
    HD UniqueProcessId;                 // The identifier of the process.
    HD InheritedFromUniqueProcessId;    // The identifier of the process that created this process. Not updated and incorrectly refers to processes with recycled identifiers.
    U4 HandleCount;                     // The current number of open handles used by the process.
    U4 SessionId;                       // The identifier of the Remote Desktop Services session under which the specified process is running.
    UA UniqueProcessKey;                // since VISTA (requires SystemExtendedProcessInformation)
    UA PeakVirtualSize;                 // The peak size, in bytes, of the virtual memory used by the process.
    UA VirtualSize;                     // The current size, in bytes, of virtual memory used by the process.
    U4 PageFaultCount;                  // The total number of page faults for data that is not currently in memory. The value wraps around to zero on average 24 hours.
    UA PeakWorkingSetSize;              // The peak size, in kilobytes, of the working set of the process.
    UA WorkingSetSize;                  // The number of pages visible to the process in physical memory. These pages are resident and available for use without triggering a page fault.
    UA QuotaPeakPagedPoolUsage;         // The peak quota charged to the process for pool usage, in bytes.
    UA QuotaPagedPoolUsage;             // The quota charged to the process for paged pool usage, in bytes.
    UA QuotaPeakNonPagedPoolUsage;      // The peak quota charged to the process for nonpaged pool usage, in bytes.
    UA QuotaNonPagedPoolUsage;          // The current quota charged to the process for nonpaged pool usage.
    UA PagefileUsage;                   // The total number of bytes of page file storage in use by the process.
    UA PeakPagefileUsage;               // The maximum number of bytes of page-file storage used by the process.
    UA PrivatePageCount;                // The number of memory pages allocated for the use by the process.
    LARGE_INTEGER_ ReadOperationCount;  // The total number of read operations performed.
    LARGE_INTEGER_ WriteOperationCount; // The total number of write operations performed.
    LARGE_INTEGER_ OtherOperationCount; // The total number of I/O operations performed other than read and write operations.
    LARGE_INTEGER_ ReadTransferCount;   // The total number of bytes read during a read operation.
    LARGE_INTEGER_ WriteTransferCount;  // The total number of bytes written during a write operation.
    LARGE_INTEGER_ OtherTransferCount;  // The total number of bytes transferred during operations other than read and write operations.
    SYSTEM_THREAD_INFORMATION_ Threads flexarr; // This type is not defined in the structure but was added for convenience.
};
struct TOKEN_GROUPS_
{
    U4 GroupCount;
    SID_AND_ATTRIBUTES_ Groups flexarr;
};
struct TOKEN_OWNER_
{
    SID_* Owner;
};
struct TOKEN_PRIMARY_GROUP_
{
    SID_* PrimaryGroup;
};
struct TOKEN_DEFAULT_DACL_
{
    ACL_* DefaultDacl;
};
struct TOKEN_SOURCE_
{
    CS SourceName[TOKEN_SOURCE_LENGTH];
    LUID_ SourceIdentifier;
};
struct IO_STATUS_BLOCK_
{
    union {
        NTSTATUS Status;
        GA Pointer;
    };
    UA Information;
};
struct PROCESS_PRIORITY_CLASS_
{
    U1 Foreground;
    U1 PriorityClass;
};
struct SYSTEM_BASIC_INFORMATION_
{
    U4 Reserved;
    U4 TimerResolution;
    U4 PageSize;
    U4 NumberOfPhysicalPages;
    U4 LowestPhysicalPageNumber;
    U4 HighestPhysicalPageNumber;
    U4 AllocationGranularity;
    UA MinimumUserModeAddress;
    UA MaximumUserModeAddress;
    UA ActiveProcessorsAffinityMask;
    U1 NumberOfProcessors;
};
struct PROCESS_CYCLE_TIME_INFORMATION_
{
    U8 AccumulatedCycles;
    U8 CurrentCycleCount;
};

// clang-format off
using LdrLoadDll_T = NTSTATUS(NTAPI*)(cx CH* DllPath, U4* DllCharacteristics, cx UNICODE_STRING_* DllName, GA* DllHandle);
using LdrUnloadDll_T = NTSTATUS(NTAPI*)(GA DllHandle);
using NtAdjustPrivilegesToken_T = NTSTATUS(NTAPI*)(HD TokenHandle, U1 DisableAllPrivileges, TOKEN_PRIVILEGES_* NewState, U4 BufferLength, TOKEN_PRIVILEGES_* PreviousState, U4* ReturnLength);
using NtAllocateVirtualMemory_T = NTSTATUS(NTAPI*)(HD ProcessHandle, GA* BaseAddress, UA ZeroBits, UA* RegionSize, U4 AllocationType, U4 PageProtection);
using NtClose_T = NTSTATUS(NTAPI*)(HD Handle);
using NtCreateEvent_T = NTSTATUS(NTAPI*)(HD* EventHandle, U4 DesiredAccess, OBJECT_ATTRIBUTES_* ObjectAttributes, EVENT_TYPE_ EventType, U1 InitialState);
using NtCreateFile_T = NTSTATUS(NTAPI*)(HD* FileHandle, U4 DesiredAccess, cx OBJECT_ATTRIBUTES_* ObjectAttributes, IO_STATUS_BLOCK_* IoStatusBlock, LARGE_INTEGER_* AllocationSize, U4 FileAttributes, U4 ShareAccess, U4 CreateDisposition, U4 CreateOptions, GA EaBuffer, U4 EaLength);
using NtCreateKeyedEvent_T = NTSTATUS(NTAPI*)(HD* KeyedEventHandle, U4 DesiredAccess, cx OBJECT_ATTRIBUTES_* ObjectAttributes, U4 Flags);
using NtCreateProcessEx_T = NTSTATUS(NTAPI*)(HD* ProcessHandle, U4 DesiredAccess, cx OBJECT_ATTRIBUTES_* ObjectAttributes, HD ParentProcess, U4 Flags, HD SectionHandle, HD DebugPort, HD TokenHandle, U4 Reserved);
using NtCreateSection_T = NTSTATUS(NTAPI*)(HD* SectionHandle, U4 DesiredAccess, cx OBJECT_ATTRIBUTES_* ObjectAttributes, LARGE_INTEGER_* MaximumSize, U4 SectionPageProtection, U4 AllocationAttributes, HD FileHandle);
using NtCreateThreadEx_T = NTSTATUS(NTAPI*)(HD* ThreadHandle, U4 DesiredAccess, cx OBJECT_ATTRIBUTES_* ObjectAttributes, HD ProcessHandle, PUSER_THREAD_START_ROUTINE_ StartRoutine, GA Argument, U4 CreateFlags, UA ZeroBits, UA StackSize, UA MaximumStackSize, PS_ATTRIBUTE_LIST_* AttributeList);
using NtCreateToken_T = NTSTATUS(NTAPI*)(HD* TokenHandle, U4 DesiredAccess, OBJECT_ATTRIBUTES_* ObjectAttributes, TOKEN_TYPE_ Type, LUID_* AuthenticationId, LARGE_INTEGER_* ExpirationTime, TOKEN_USER_* User, TOKEN_GROUPS_* Groups, TOKEN_PRIVILEGES_* Privileges, TOKEN_OWNER_* Owner, TOKEN_PRIMARY_GROUP_* PrimaryGroup, TOKEN_DEFAULT_DACL_* DefaultDacl, TOKEN_SOURCE_* Source);
using NtCreateUserProcess_T = NTSTATUS(NTAPI*)(HD* ProcessHandle, HD* ThreadHandle, U4 ProcessDesiredAccess, U4 ThreadDesiredAccess, cx OBJECT_ATTRIBUTES_* ProcessObjectAttributes, cx OBJECT_ATTRIBUTES_* ThreadObjectAttributes, U4 ProcessFlags, U4 ThreadFlags, RTL_USER_PROCESS_PARAMETERS_* ProcessParameters, PS_CREATE_INFO_* CreateInfo, PS_ATTRIBUTE_LIST_* AttributeList);
using NtDelayExecution_T = NTSTATUS(NTAPI*)(U1 Alertable, LARGE_INTEGER_* DelayInterval);
using NtDuplicateObject_T = NTSTATUS(NTAPI*)(HD SourceProcessHandle, HD SourceHandle, HD TargetProcessHandle, HD* TargetHandle, U4 DesiredAccess, U4 HandleAttributes, U4 Options);
using NtDuplicateToken_T = NTSTATUS(NTAPI*)(HD ExistingTokenHandle, U4 DesiredAccess, OBJECT_ATTRIBUTES_* ObjectAttributes, U1 EffectiveOnly, TOKEN_TYPE_ Type, HD* NewTokenHandle);
using NtFlushInstructionCache_T = NTSTATUS(NTAPI*)(HD ProcessHandle, GA BaseAddress, UA RegionSize);
using NtFreeVirtualMemory_T = NTSTATUS(NTAPI*)(HD ProcessHandle, GA* BaseAddress, UA* RegionSize, U4 FreeType);
using NtImpersonateThread_T = NTSTATUS(NTAPI*)(HD ServerThreadHandle, HD ClientThreadHandle, SECURITY_QUALITY_OF_SERVICE_* SecurityQos);
using NtLoadDriver_T = NTSTATUS(NTAPI*)(cx UNICODE_STRING_* DriverServiceName);
using NtMapViewOfSection_T = NTSTATUS(NTAPI*)(HD SectionHandle, HD ProcessHandle, GA* BaseAddress, UA ZeroBits, UA CommitSize, LARGE_INTEGER_* SectionOffset, UA* ViewSize, SECTION_INHERIT_ InheritDisposition, U4 AllocationType, U4 PageProtection);
using NtOpenKeyEx_T = NTSTATUS(NTAPI*)(HD* KeyHandle, U4 DesiredAccess, OBJECT_ATTRIBUTES_* ObjectAttributes, U4 OpenOptions);
using NtOpenKeyedEvent_T = NTSTATUS(NTAPI*)(HD* KeyedEventHandle, U4 DesiredAccess, cx OBJECT_ATTRIBUTES_* ObjectAttributes);
using NtOpenProcessTokenEx_T = NTSTATUS(NTAPI*)(HD ProcessHandle, U4 DesiredAccess, U4 HandleAttributes, HD* TokenHandle);
using NtOpenProcess_T = NTSTATUS(NTAPI*)(HD* ProcessHandle, U4 DesiredAccess, cx OBJECT_ATTRIBUTES_* ObjectAttributes, CLIENT_ID_* ClientId);
using NtOpenThreadTokenEx_T = NTSTATUS(NTAPI*)(HD ThreadHandle, U4 DesiredAccess, U1 OpenAsSelf, U4 HandleAttributes, HD* TokenHandle);
using NtOpenThread_T = NTSTATUS(NTAPI*)(HD* ThreadHandle, U4 DesiredAccess, cx OBJECT_ATTRIBUTES_* ObjectAttributes, CLIENT_ID_* ClientId);
using NtQueryDirectoryFile_T = NTSTATUS(NTAPI*)(HD FileHandle, HD Event, PIO_APC_ROUTINE_ ApcRoutine, GA ApcContext, IO_STATUS_BLOCK_* IoStatusBlock, GA FileInformation, U4 Length, FILE_INFORMATION_CLASS_ FileInformationClass, U1 ReturnSingleEntry, cx UNICODE_STRING_* FileName, U1 RestartScan);
using NtQueryInformationFile_T = NTSTATUS(NTAPI*)(HD FileHandle, IO_STATUS_BLOCK_* IoStatusBlock, GA FileInformation, U4 Length, FILE_INFORMATION_CLASS_ FileInformationClass);
using NtQueryInformationProcess_T = NTSTATUS(NTAPI*)(HD ProcessHandle, PROCESSINFOCLASS_ ProcessInformationClass, GA ProcessInformation, U4 ProcessInformationLength, U4* ReturnLength);
using NtQueryInformationThread_T = NTSTATUS(NTAPI*)(HD ThreadHandle, THREADINFOCLASS_ ThreadInformationClass, GA ThreadInformation, U4 ThreadInformationLength, U4* ReturnLength);
using NtQueryInformationToken_T = NTSTATUS(NTAPI*)(HD TokenHandle, TOKEN_INFORMATION_CLASS_ TokenInformationClass, GA TokenInformation, U4 TokenInformationLength, U4* ReturnLength);
using NtQueryObject_T = NTSTATUS(NTAPI*)(HD Handle, OBJECT_INFORMATION_CLASS_ ObjectInformationClass, GA ObjectInformation, U4 ObjectInformationLength, U4* ReturnLength);
using NtQuerySystemInformation_T = NTSTATUS(NTAPI*)(SYSTEM_INFORMATION_CLASS_ SystemInformationClass, GA SystemInformation, U4 SystemInformationLength, U4* ReturnLength);
using NtQueryTimerResolution_T = NTSTATUS(NTAPI*)(U4* MaximumTime, U4* MinimumTime, U4* CurrentTime);
using NtQueryValueKey_T = NTSTATUS(NTAPI*)(HD KeyHandle, cx UNICODE_STRING_* ValueName, KEY_VALUE_INFORMATION_CLASS_ KeyValueInformationClass, GA KeyValueInformation, U4 Length, U4* ResultLength);
using NtRaiseHardError_T = NTSTATUS(NTAPI*)(NTSTATUS ErrorStatus, U4 NumberOfParameters, U4 UnicodeStringParameterMask, UA* Parameters, U4 ValidResponseOptions, U4* Response);
using NtReadFile_T = NTSTATUS(NTAPI*)(HD FileHandle, HD Event, PIO_APC_ROUTINE_ ApcRoutine, GA ApcContext, IO_STATUS_BLOCK_* IoStatusBlock, GA Buffer, U4 Length, LARGE_INTEGER_* ByteOffset, U4* Key);
using NtReadVirtualMemory_T = NTSTATUS(NTAPI*)(HD ProcessHandle, CXGA BaseAddress, GA Buffer, UA NumberOfBytesToRead, UA* NumberOfBytesRead);
using NtReleaseKeyedEvent_T = NTSTATUS(NTAPI*)(HD KeyedEventHandle, GA KeyValue, U1 Alertable, LARGE_INTEGER_* Timeout);
using NtResetEvent_T = NTSTATUS(NTAPI*)(HD EventHandle, S4* PreviousState);
using NtSetEvent_T = NTSTATUS(NTAPI*)(HD EventHandle, S4* PreviousState);
using NtSetInformationFile_T = NTSTATUS(NTAPI*)(HD FileHandle, IO_STATUS_BLOCK_* IoStatusBlock, GA FileInformation, U4 Length, FILE_INFORMATION_CLASS_ FileInformationClass);
using NtSetInformationProcess_T = NTSTATUS(NTAPI*)(HD ProcessHandle, PROCESSINFOCLASS_ ProcessInformationClass, GA ProcessInformation, U4 ProcessInformationLength);
using NtSetInformationThread_T = NTSTATUS(NTAPI*)(HD ThreadHandle, THREADINFOCLASS_ ThreadInformationClass, GA ThreadInformation, U4 ThreadInformationLength);
using NtSetInformationToken_T = NTSTATUS(NTAPI*)(HD TokenHandle, TOKEN_INFORMATION_CLASS_ TokenInformationClass, GA TokenInformation, U4 TokenInformationLength);
using NtSetTimerResolution_T = NTSTATUS(NTAPI*)(U4 DesiredTime, U1 SetResolution, U4* ActualTime);
using NtSetValueKey_T = NTSTATUS(NTAPI*)(HD KeyHandle, cx UNICODE_STRING_* ValueName, U4 TitleIndex, U4 Type, GA Data, U4 DataSize);
using NtShutdownSystem_T = NTSTATUS(NTAPI*)(SHUTDOWN_ACTION_ Action);
using NtTerminateProcess_T = NTSTATUS(NTAPI*)(HD ProcessHandle, NTSTATUS ExitStatus);
using NtTerminateThread_T = NTSTATUS(NTAPI*)(HD ThreadHandle, NTSTATUS ExitStatus);
using NtUnloadDriver_T = NTSTATUS(NTAPI*)(cx UNICODE_STRING_* DriverServiceName);
using NtWaitForKeyedEvent_T = NTSTATUS(NTAPI*)(HD KeyedEventHandle, GA KeyValue, U1 Alertable, LARGE_INTEGER_* Timeout);
using NtWaitForSingleObject_T = NTSTATUS(NTAPI*)(HD Handle, U1 Alertable, LARGE_INTEGER_* Timeout);
using NtWriteFile_T = NTSTATUS(NTAPI*)(HD FileHandle, HD Event, PIO_APC_ROUTINE_ ApcRoutine, GA ApcContext, IO_STATUS_BLOCK_* IoStatusBlock, GA Buffer, U4 Length, LARGE_INTEGER_* ByteOffset, U4* Key);
using NtWriteVirtualMemory_T = NTSTATUS(NTAPI*)(HD ProcessHandle, GA BaseAddress, CXGA Buffer, UA NumberOfBytesToWrite, UA* NumberOfBytesWritten);
using NtYieldExecution_T = NTSTATUS(NTAPI*)();
using RtlAcquirePebLock_T = NTSTATUS(NTAPI*)();
using RtlAdjustPrivilege_T = NTSTATUS(NTAPI*)(U4 Privilege, U1 Enable, U1 Client, U1* WasEnabled);
using RtlExitUserProcess_T = NT(NTAPI*)(NTSTATUS ExitStatus);
using RtlReleasePebLock_T = NTSTATUS(NTAPI*)();
// clang-format on

// Note: this creates global variables & initializes them
#define _UNI_NT_INIT_FN(fnName) fnName##_T fnName##_ = NUL

_UNI_NT_INIT_FN(LdrLoadDll);
_UNI_NT_INIT_FN(LdrUnloadDll);
_UNI_NT_INIT_FN(NtAdjustPrivilegesToken);
_UNI_NT_INIT_FN(NtAllocateVirtualMemory);
_UNI_NT_INIT_FN(NtClose);
_UNI_NT_INIT_FN(NtCreateEvent);
_UNI_NT_INIT_FN(NtCreateFile);
_UNI_NT_INIT_FN(NtCreateKeyedEvent);
_UNI_NT_INIT_FN(NtCreateProcessEx);
_UNI_NT_INIT_FN(NtCreateSection);
_UNI_NT_INIT_FN(NtCreateThreadEx);
_UNI_NT_INIT_FN(NtCreateToken);
_UNI_NT_INIT_FN(NtCreateUserProcess);
_UNI_NT_INIT_FN(NtDelayExecution);
_UNI_NT_INIT_FN(NtDuplicateObject);
_UNI_NT_INIT_FN(NtDuplicateToken);
_UNI_NT_INIT_FN(NtFlushInstructionCache);
_UNI_NT_INIT_FN(NtFreeVirtualMemory);
_UNI_NT_INIT_FN(NtImpersonateThread);
_UNI_NT_INIT_FN(NtLoadDriver);
_UNI_NT_INIT_FN(NtMapViewOfSection);
_UNI_NT_INIT_FN(NtOpenKeyEx);
_UNI_NT_INIT_FN(NtOpenKeyedEvent);
_UNI_NT_INIT_FN(NtOpenProcess);
_UNI_NT_INIT_FN(NtOpenProcessTokenEx);
_UNI_NT_INIT_FN(NtOpenThread);
_UNI_NT_INIT_FN(NtOpenThreadTokenEx);
_UNI_NT_INIT_FN(NtQueryDirectoryFile);
_UNI_NT_INIT_FN(NtQueryInformationFile);
_UNI_NT_INIT_FN(NtQueryInformationProcess);
_UNI_NT_INIT_FN(NtQueryInformationThread);
_UNI_NT_INIT_FN(NtQueryInformationToken);
_UNI_NT_INIT_FN(NtQueryObject);
_UNI_NT_INIT_FN(NtQuerySystemInformation);
_UNI_NT_INIT_FN(NtQueryTimerResolution);
_UNI_NT_INIT_FN(NtQueryValueKey);
_UNI_NT_INIT_FN(NtRaiseHardError);
_UNI_NT_INIT_FN(NtReadFile);
_UNI_NT_INIT_FN(NtReadVirtualMemory);
_UNI_NT_INIT_FN(NtReleaseKeyedEvent);
_UNI_NT_INIT_FN(NtResetEvent);
_UNI_NT_INIT_FN(NtSetEvent);
_UNI_NT_INIT_FN(NtSetInformationFile);
_UNI_NT_INIT_FN(NtSetInformationProcess);
_UNI_NT_INIT_FN(NtSetInformationThread);
_UNI_NT_INIT_FN(NtSetInformationToken);
_UNI_NT_INIT_FN(NtSetTimerResolution);
_UNI_NT_INIT_FN(NtSetValueKey);
_UNI_NT_INIT_FN(NtShutdownSystem);
_UNI_NT_INIT_FN(NtTerminateProcess);
_UNI_NT_INIT_FN(NtTerminateThread);
_UNI_NT_INIT_FN(NtUnloadDriver);
_UNI_NT_INIT_FN(NtWaitForKeyedEvent);
_UNI_NT_INIT_FN(NtWaitForSingleObject);
_UNI_NT_INIT_FN(NtWriteFile);
_UNI_NT_INIT_FN(NtWriteVirtualMemory);
_UNI_NT_INIT_FN(NtYieldExecution);
_UNI_NT_INIT_FN(RtlAcquirePebLock);
_UNI_NT_INIT_FN(RtlAdjustPrivilege);
_UNI_NT_INIT_FN(RtlExitUserProcess);
_UNI_NT_INIT_FN(RtlReleasePebLock);

dfa ER UniNtLoad();
