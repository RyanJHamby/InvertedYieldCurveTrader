// Unity Build generated by CMake
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/LookoutEquipmentEndpointProvider.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/LookoutEquipmentEndpointRules.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/LookoutEquipmentErrorMarshaller.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/LookoutEquipmentErrors.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/LookoutEquipmentRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/CategoricalValues.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/CountPercent.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/CreateDatasetRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/CreateDatasetResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/CreateInferenceSchedulerRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/CreateInferenceSchedulerResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/CreateLabelGroupRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/CreateLabelGroupResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/CreateLabelRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/CreateLabelResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/CreateModelRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/CreateModelResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DataIngestionJobSummary.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DataPreProcessingConfiguration.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DataQualitySummary.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DataUploadFrequency.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DatasetSchema.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DatasetStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DatasetSummary.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DeleteDatasetRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DeleteInferenceSchedulerRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DeleteLabelGroupRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DeleteLabelRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DeleteModelRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DeleteResourcePolicyRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DescribeDataIngestionJobRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DescribeDataIngestionJobResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DescribeDatasetRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DescribeDatasetResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DescribeInferenceSchedulerRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DescribeInferenceSchedulerResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DescribeLabelGroupRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DescribeLabelGroupResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DescribeLabelRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DescribeLabelResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DescribeModelRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DescribeModelResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DescribeModelVersionRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DescribeModelVersionResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DescribeResourcePolicyRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DescribeResourcePolicyResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/DuplicateTimestamps.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ImportDatasetRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ImportDatasetResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ImportModelVersionRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ImportModelVersionResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/InferenceEventSummary.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/InferenceExecutionStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/InferenceExecutionSummary.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/InferenceInputConfiguration.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/InferenceInputNameConfiguration.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/InferenceOutputConfiguration.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/InferenceS3InputConfiguration.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/InferenceS3OutputConfiguration.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/InferenceSchedulerStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/InferenceSchedulerSummary.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/IngestedFilesSummary.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/IngestionInputConfiguration.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/IngestionJobStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/IngestionS3InputConfiguration.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/InsufficientSensorData.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/InvalidSensorData.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/LabelGroupSummary.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/LabelRating.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/LabelSummary.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/LabelsInputConfiguration.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/LabelsS3InputConfiguration.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/LargeTimestampGaps.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/LatestInferenceResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListDataIngestionJobsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListDataIngestionJobsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListDatasetsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListDatasetsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListInferenceEventsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListInferenceEventsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListInferenceExecutionsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListInferenceExecutionsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListInferenceSchedulersRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListInferenceSchedulersResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListLabelGroupsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListLabelGroupsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListLabelsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListLabelsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListModelVersionsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListModelVersionsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListModelsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListModelsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListSensorStatisticsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListSensorStatisticsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListTagsForResourceRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ListTagsForResourceResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/MissingCompleteSensorData.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/MissingSensorData.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ModelStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ModelSummary.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ModelVersionSourceType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ModelVersionStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/ModelVersionSummary.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/MonotonicValues.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/Monotonicity.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/MultipleOperatingModes.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/PutResourcePolicyRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/PutResourcePolicyResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/S3Object.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/SensorStatisticsSummary.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/SensorsWithShortDateRange.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/StartDataIngestionJobRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/StartDataIngestionJobResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/StartInferenceSchedulerRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/StartInferenceSchedulerResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/StatisticalIssueStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/StopInferenceSchedulerRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/StopInferenceSchedulerResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/Tag.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/TagResourceRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/TagResourceResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/TargetSamplingRate.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/UnsupportedTimestamps.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/UntagResourceRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/UntagResourceResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/UpdateActiveModelVersionRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/UpdateActiveModelVersionResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/UpdateInferenceSchedulerRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/model/UpdateLabelGroupRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-lookoutequipment/source/LookoutEquipmentClient.cpp>
