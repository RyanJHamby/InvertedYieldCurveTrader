// Unity Build generated by CMake
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/DeviceFarmEndpointProvider.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/DeviceFarmEndpointRules.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/DeviceFarmErrorMarshaller.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/DeviceFarmErrors.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/DeviceFarmRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/AccountSettings.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/Artifact.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ArtifactCategory.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ArtifactType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/BillingMethod.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CPU.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/Counters.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CreateDevicePoolRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CreateDevicePoolResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CreateInstanceProfileRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CreateInstanceProfileResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CreateNetworkProfileRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CreateNetworkProfileResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CreateProjectRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CreateProjectResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CreateRemoteAccessSessionConfiguration.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CreateRemoteAccessSessionRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CreateRemoteAccessSessionResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CreateTestGridProjectRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CreateTestGridProjectResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CreateTestGridUrlRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CreateTestGridUrlResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CreateUploadRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CreateUploadResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CreateVPCEConfigurationRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CreateVPCEConfigurationResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CurrencyCode.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/CustomerArtifactPaths.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeleteDevicePoolRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeleteDevicePoolResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeleteInstanceProfileRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeleteInstanceProfileResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeleteNetworkProfileRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeleteNetworkProfileResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeleteProjectRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeleteProjectResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeleteRemoteAccessSessionRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeleteRemoteAccessSessionResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeleteRunRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeleteRunResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeleteTestGridProjectRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeleteTestGridProjectResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeleteUploadRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeleteUploadResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeleteVPCEConfigurationRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeleteVPCEConfigurationResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/Device.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeviceAttribute.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeviceAvailability.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeviceFilter.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeviceFilterAttribute.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeviceFormFactor.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeviceInstance.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeviceMinutes.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DevicePlatform.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DevicePool.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DevicePoolCompatibilityResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DevicePoolType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeviceSelectionConfiguration.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/DeviceSelectionResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ExecutionConfiguration.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ExecutionResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ExecutionResultCode.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ExecutionStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetAccountSettingsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetAccountSettingsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetDeviceInstanceRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetDeviceInstanceResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetDevicePoolCompatibilityRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetDevicePoolCompatibilityResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetDevicePoolRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetDevicePoolResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetDeviceRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetDeviceResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetInstanceProfileRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetInstanceProfileResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetJobRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetJobResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetNetworkProfileRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetNetworkProfileResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetOfferingStatusRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetOfferingStatusResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetProjectRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetProjectResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetRemoteAccessSessionRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetRemoteAccessSessionResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetRunRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetRunResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetSuiteRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetSuiteResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetTestGridProjectRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetTestGridProjectResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetTestGridSessionRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetTestGridSessionResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetTestRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetTestResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetUploadRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetUploadResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetVPCEConfigurationRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/GetVPCEConfigurationResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/IncompatibilityMessage.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/InstallToRemoteAccessSessionRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/InstallToRemoteAccessSessionResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/InstanceProfile.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/InstanceStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/InteractionMode.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/Job.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListArtifactsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListArtifactsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListDeviceInstancesRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListDeviceInstancesResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListDevicePoolsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListDevicePoolsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListDevicesRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListDevicesResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListInstanceProfilesRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListInstanceProfilesResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListJobsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListJobsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListNetworkProfilesRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListNetworkProfilesResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListOfferingPromotionsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListOfferingPromotionsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListOfferingTransactionsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListOfferingTransactionsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListOfferingsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListOfferingsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListProjectsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListProjectsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListRemoteAccessSessionsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListRemoteAccessSessionsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListRunsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListRunsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListSamplesRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListSamplesResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListSuitesRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListSuitesResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListTagsForResourceRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListTagsForResourceResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListTestGridProjectsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListTestGridProjectsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListTestGridSessionActionsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListTestGridSessionActionsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListTestGridSessionArtifactsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListTestGridSessionArtifactsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListTestGridSessionsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListTestGridSessionsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListTestsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListTestsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListUniqueProblemsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListUniqueProblemsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListUploadsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListUploadsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListVPCEConfigurationsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ListVPCEConfigurationsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/Location.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/MonetaryAmount.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/NetworkProfile.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/NetworkProfileType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/Offering.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/OfferingPromotion.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/OfferingStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/OfferingTransaction.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/OfferingTransactionType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/OfferingType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/Problem.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ProblemDetail.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/Project.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/PurchaseOfferingRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/PurchaseOfferingResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/Radios.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/RecurringCharge.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/RecurringChargeFrequency.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/RemoteAccessSession.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/RenewOfferingRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/RenewOfferingResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/Resolution.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/Rule.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/RuleOperator.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/Run.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/Sample.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/SampleType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ScheduleRunConfiguration.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ScheduleRunRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ScheduleRunResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/ScheduleRunTest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/StopJobRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/StopJobResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/StopRemoteAccessSessionRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/StopRemoteAccessSessionResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/StopRunRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/StopRunResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/Suite.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/Tag.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/TagOperationException.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/TagPolicyException.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/TagResourceRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/TagResourceResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/Test.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/TestGridProject.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/TestGridSession.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/TestGridSessionAction.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/TestGridSessionArtifact.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/TestGridSessionArtifactCategory.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/TestGridSessionArtifactType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/TestGridSessionStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/TestGridVpcConfig.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/TestType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/TooManyTagsException.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/TrialMinutes.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UniqueProblem.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UntagResourceRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UntagResourceResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UpdateDeviceInstanceRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UpdateDeviceInstanceResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UpdateDevicePoolRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UpdateDevicePoolResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UpdateInstanceProfileRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UpdateInstanceProfileResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UpdateNetworkProfileRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UpdateNetworkProfileResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UpdateProjectRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UpdateProjectResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UpdateTestGridProjectRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UpdateTestGridProjectResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UpdateUploadRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UpdateUploadResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UpdateVPCEConfigurationRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UpdateVPCEConfigurationResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/Upload.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UploadCategory.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UploadStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/UploadType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/VPCEConfiguration.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/model/VpcConfig.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-devicefarm/source/DeviceFarmClient.cpp>
