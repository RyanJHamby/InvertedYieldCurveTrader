// Unity Build generated by CMake
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/ACMPCAEndpointProvider.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/ACMPCAEndpointRules.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/ACMPCAErrorMarshaller.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/ACMPCAErrors.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/ACMPCARequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/ASN1Subject.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/AccessDescription.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/AccessMethod.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/AccessMethodType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/ActionType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/ApiPassthrough.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/AuditReportResponseFormat.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/AuditReportStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/CertificateAuthority.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/CertificateAuthorityConfiguration.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/CertificateAuthorityStatus.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/CertificateAuthorityType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/CertificateAuthorityUsageMode.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/CreateCertificateAuthorityAuditReportRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/CreateCertificateAuthorityAuditReportResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/CreateCertificateAuthorityRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/CreateCertificateAuthorityResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/CreatePermissionRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/CrlConfiguration.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/CsrExtensions.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/CustomAttribute.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/CustomExtension.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/DeleteCertificateAuthorityRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/DeletePermissionRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/DeletePolicyRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/DescribeCertificateAuthorityAuditReportRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/DescribeCertificateAuthorityAuditReportResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/DescribeCertificateAuthorityRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/DescribeCertificateAuthorityResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/EdiPartyName.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/ExtendedKeyUsage.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/ExtendedKeyUsageType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/Extensions.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/FailureReason.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/GeneralName.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/GetCertificateAuthorityCertificateRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/GetCertificateAuthorityCertificateResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/GetCertificateAuthorityCsrRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/GetCertificateAuthorityCsrResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/GetCertificateRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/GetCertificateResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/GetPolicyRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/GetPolicyResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/ImportCertificateAuthorityCertificateRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/IssueCertificateRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/IssueCertificateResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/KeyAlgorithm.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/KeyStorageSecurityStandard.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/KeyUsage.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/ListCertificateAuthoritiesRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/ListCertificateAuthoritiesResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/ListPermissionsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/ListPermissionsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/ListTagsRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/ListTagsResult.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/OcspConfiguration.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/OtherName.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/Permission.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/PolicyInformation.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/PolicyQualifierId.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/PolicyQualifierInfo.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/PutPolicyRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/Qualifier.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/ResourceOwner.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/RestoreCertificateAuthorityRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/RevocationConfiguration.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/RevocationReason.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/RevokeCertificateRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/S3ObjectAcl.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/SigningAlgorithm.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/Tag.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/TagCertificateAuthorityRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/UntagCertificateAuthorityRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/UpdateCertificateAuthorityRequest.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/Validity.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/model/ValidityPeriodType.cpp>
#include </Users/ryanhamby/Desktop/aws-sdk-cpp-main/generated/src/aws-cpp-sdk-acm-pca/source/ACMPCAClient.cpp>
