// Unity Build generated by CMake
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/DrsEndpointProvider.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/DrsEndpointRules.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/DrsErrorMarshaller.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/DrsErrors.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/DrsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/AccessDeniedException.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/Account.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/AssociateSourceNetworkStackRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/AssociateSourceNetworkStackResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/CPU.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ConflictException.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ConversionProperties.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/CreateExtendedSourceServerRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/CreateExtendedSourceServerResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/CreateLaunchConfigurationTemplateRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/CreateLaunchConfigurationTemplateResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/CreateReplicationConfigurationTemplateRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/CreateReplicationConfigurationTemplateResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/CreateSourceNetworkRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/CreateSourceNetworkResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DataReplicationError.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DataReplicationErrorString.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DataReplicationInfo.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DataReplicationInfoReplicatedDisk.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DataReplicationInitiation.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DataReplicationInitiationStep.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DataReplicationInitiationStepName.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DataReplicationInitiationStepStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DataReplicationState.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DeleteJobRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DeleteJobResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DeleteLaunchConfigurationTemplateRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DeleteLaunchConfigurationTemplateResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DeleteRecoveryInstanceRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DeleteReplicationConfigurationTemplateRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DeleteReplicationConfigurationTemplateResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DeleteSourceNetworkRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DeleteSourceNetworkResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DeleteSourceServerRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DeleteSourceServerResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeJobLogItemsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeJobLogItemsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeJobsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeJobsRequestFilters.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeJobsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeLaunchConfigurationTemplatesRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeLaunchConfigurationTemplatesResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeRecoveryInstancesRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeRecoveryInstancesRequestFilters.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeRecoveryInstancesResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeRecoverySnapshotsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeRecoverySnapshotsRequestFilters.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeRecoverySnapshotsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeReplicationConfigurationTemplatesRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeReplicationConfigurationTemplatesResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeSourceNetworksRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeSourceNetworksRequestFilters.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeSourceNetworksResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeSourceServersRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeSourceServersRequestFilters.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DescribeSourceServersResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DisconnectRecoveryInstanceRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DisconnectSourceServerRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/DisconnectSourceServerResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/Disk.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/EC2InstanceState.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/EventResourceData.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ExportSourceNetworkCfnTemplateRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ExportSourceNetworkCfnTemplateResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ExtensionStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/FailbackLaunchType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/FailbackReplicationError.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/FailbackState.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/GetFailbackReplicationConfigurationRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/GetFailbackReplicationConfigurationResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/GetLaunchConfigurationRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/GetLaunchConfigurationResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/GetReplicationConfigurationRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/GetReplicationConfigurationResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/IdentificationHints.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/InitializeServiceRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/InitializeServiceResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/InitiatedBy.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/InternalServerException.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/Job.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/JobLog.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/JobLogEvent.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/JobLogEventData.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/JobStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/JobType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/LastLaunchResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/LastLaunchType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/LaunchConfigurationTemplate.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/LaunchDisposition.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/LaunchStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/Licensing.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/LifeCycle.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/LifeCycleLastLaunch.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/LifeCycleLastLaunchInitiated.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ListExtensibleSourceServersRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ListExtensibleSourceServersResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ListStagingAccountsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ListStagingAccountsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ListTagsForResourceRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ListTagsForResourceResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/NetworkInterface.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/OS.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/OriginEnvironment.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/PITPolicyRule.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/PITPolicyRuleUnits.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ParticipatingResource.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ParticipatingResourceID.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ParticipatingServer.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/RecoveryInstance.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/RecoveryInstanceDataReplicationError.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/RecoveryInstanceDataReplicationInfo.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/RecoveryInstanceDataReplicationInfoReplicatedDisk.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/RecoveryInstanceDataReplicationInitiation.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/RecoveryInstanceDataReplicationInitiationStep.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/RecoveryInstanceDataReplicationInitiationStepName.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/RecoveryInstanceDataReplicationInitiationStepStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/RecoveryInstanceDataReplicationState.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/RecoveryInstanceDisk.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/RecoveryInstanceFailback.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/RecoveryInstanceProperties.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/RecoveryLifeCycle.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/RecoveryResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/RecoverySnapshot.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/RecoverySnapshotsOrder.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ReplicationConfigurationDataPlaneRouting.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ReplicationConfigurationDefaultLargeStagingDiskType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ReplicationConfigurationEbsEncryption.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ReplicationConfigurationReplicatedDisk.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ReplicationConfigurationReplicatedDiskStagingDiskType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ReplicationConfigurationTemplate.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ReplicationDirection.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ReplicationStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ResourceNotFoundException.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ReverseReplicationRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ReverseReplicationResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ServiceQuotaExceededException.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/SourceCloudProperties.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/SourceNetwork.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/SourceNetworkData.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/SourceProperties.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/SourceServer.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/StagingArea.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/StagingSourceServer.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/StartFailbackLaunchRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/StartFailbackLaunchResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/StartRecoveryRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/StartRecoveryRequestSourceServer.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/StartRecoveryResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/StartReplicationRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/StartReplicationResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/StartSourceNetworkRecoveryRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/StartSourceNetworkRecoveryRequestNetworkEntry.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/StartSourceNetworkRecoveryResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/StartSourceNetworkReplicationRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/StartSourceNetworkReplicationResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/StopFailbackRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/StopReplicationRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/StopReplicationResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/StopSourceNetworkReplicationRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/StopSourceNetworkReplicationResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/TagResourceRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/TargetInstanceTypeRightSizingMethod.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/TerminateRecoveryInstancesRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/TerminateRecoveryInstancesResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ThrottlingException.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/UninitializedAccountException.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/UntagResourceRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/UpdateFailbackReplicationConfigurationRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/UpdateLaunchConfigurationRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/UpdateLaunchConfigurationResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/UpdateLaunchConfigurationTemplateRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/UpdateLaunchConfigurationTemplateResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/UpdateReplicationConfigurationRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/UpdateReplicationConfigurationResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/UpdateReplicationConfigurationTemplateRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/UpdateReplicationConfigurationTemplateResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ValidationException.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ValidationExceptionField.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/model/ValidationExceptionReason.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-drs/source/DrsClient.cpp>
