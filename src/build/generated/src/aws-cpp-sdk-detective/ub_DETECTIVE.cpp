// Unity Build generated by CMake
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/DetectiveEndpointProvider.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/DetectiveEndpointRules.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/DetectiveErrorMarshaller.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/DetectiveErrors.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/DetectiveRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/AcceptInvitationRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/AccessDeniedException.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/Account.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/Administrator.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/BatchGetGraphMemberDatasourcesRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/BatchGetGraphMemberDatasourcesResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/BatchGetMembershipDatasourcesRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/BatchGetMembershipDatasourcesResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/CreateGraphRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/CreateGraphResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/CreateMembersRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/CreateMembersResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/DatasourcePackage.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/DatasourcePackageIngestDetail.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/DatasourcePackageIngestState.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/DatasourcePackageUsageInfo.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/DeleteGraphRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/DeleteMembersRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/DeleteMembersResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/DescribeOrganizationConfigurationRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/DescribeOrganizationConfigurationResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/DisassociateMembershipRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/EnableOrganizationAdminAccountRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/ErrorCode.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/GetMembersRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/GetMembersResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/Graph.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/InvitationType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/ListDatasourcePackagesRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/ListDatasourcePackagesResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/ListGraphsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/ListGraphsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/ListInvitationsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/ListInvitationsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/ListMembersRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/ListMembersResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/ListOrganizationAdminAccountsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/ListOrganizationAdminAccountsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/ListTagsForResourceRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/ListTagsForResourceResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/MemberDetail.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/MemberDisabledReason.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/MemberStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/MembershipDatasources.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/RejectInvitationRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/ServiceQuotaExceededException.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/StartMonitoringMemberRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/TagResourceRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/TagResourceResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/TimestampForCollection.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/UnprocessedAccount.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/UnprocessedGraph.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/UntagResourceRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/UntagResourceResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/UpdateDatasourcePackagesRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/UpdateOrganizationConfigurationRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/model/ValidationException.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-detective/source/DetectiveClient.cpp>
