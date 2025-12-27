# InvertedYieldCurveTrader CDK Infrastructure

AWS CDK infrastructure for deploying the InvertedYieldCurveTrader Lambda application across multiple environments (dev, staging, prod, etc.).

## Architecture

The CDK is organized into four independent stacks:

- **S3Stack**: Creates S3 bucket for storing daily analysis results
- **IamStack**: Creates Lambda execution role with S3 permissions
- **LambdaStack**: Deploys the compiled Lambda function with API key environment variables
- **EventBridgeStack**: Creates daily trigger rule that executes Lambda at 12:00 UTC

Each stack is environment-aware and produces logically-named resources:
- Bucket: `inverted-yield-trader-{environment}-results`
- Role: `inverted-yield-trader-{environment}-lambda-role`
- Function: `InvertedYieldTrader{Environment}` (e.g., InvertedYieldTraderProd)
- Rule: `InvertedYieldTrader{Environment}DailyTrigger`

## Prerequisites

1. **Node.js**: 16.x or later
2. **AWS CLI**: Configured with credentials and default region
3. **AWS Account**: With permissions to create Lambda, IAM, S3, EventBridge resources
4. **Lambda Binary**: `lambda-function.zip` must be present at `../lambda-function.zip`
   - This is the compiled binary from Phase 2 (Docker build)
5. **API Keys**: FRED API key and Alpha Vantage API key

## Installation

1. Navigate to the cdk directory:
```bash
cd cdk
```

2. Install dependencies:
```bash
npm install
```

3. Ensure the Lambda binary is available:
```bash
# Check that lambda-function.zip exists at the project root
ls ../lambda-function.zip
```

## Deployment

### Deploy to Development Environment

```bash
npm run deploy -- \
  --context environment=dev \
  --context fredApiKey=YOUR_FRED_API_KEY \
  --context alphaVantageApiKey=YOUR_ALPHA_VANTAGE_API_KEY
```

### Deploy to Production Environment

```bash
npm run deploy -- \
  --context environment=prod \
  --context fredApiKey=YOUR_FRED_API_KEY \
  --context alphaVantageApiKey=YOUR_ALPHA_VANTAGE_API_KEY
```

### Deploy to Custom Region

```bash
npm run deploy -- \
  --context environment=prod \
  --context region=eu-west-1 \
  --context fredApiKey=YOUR_FRED_API_KEY \
  --context alphaVantageApiKey=YOUR_ALPHA_VANTAGE_API_KEY
```

## Other Commands

### View CloudFormation Template (Synth)

Generate the CloudFormation template without deploying:

```bash
npm run synth -- \
  --context environment=dev \
  --context fredApiKey=YOUR_FRED_API_KEY \
  --context alphaVantageApiKey=YOUR_ALPHA_VANTAGE_API_KEY
```

### Preview Changes (Diff)

See what would change if you deployed:

```bash
npm run diff -- \
  --context environment=prod \
  --context fredApiKey=YOUR_FRED_API_KEY \
  --context alphaVantageApiKey=YOUR_ALPHA_VANTAGE_API_KEY
```

### Destroy Stacks

Remove all deployed resources:

```bash
npm run destroy -- \
  --context environment=dev
```

## Environment Variables Alternative

Instead of passing context values via CLI, you can set environment variables:

```bash
export ENVIRONMENT=prod
export AWS_REGION=us-east-1
export FRED_API_KEY=your_fred_key
export ALPHA_VANTAGE_API_KEY=your_alpha_vantage_key

npm run deploy
```

## Multi-Environment Deployment

Deploy the same application to multiple environments with a script:

```bash
#!/bin/bash

FRED_KEY="your_fred_api_key"
ALPHA_KEY="your_alpha_vantage_key"

for ENV in dev staging prod; do
  echo "Deploying to $ENV..."
  npm run deploy -- \
    --context environment=$ENV \
    --context fredApiKey=$FRED_KEY \
    --context alphaVantageApiKey=$ALPHA_KEY
done
```

## Stack Outputs

After deployment, CloudFormation outputs the following values for each stack:

- **S3Stack-BucketName**: Name of the results bucket
- **IamStack-LambdaRoleArn**: ARN of the Lambda execution role
- **LambdaStack-FunctionArn**: ARN of the deployed Lambda function
- **LambdaStack-FunctionName**: Name of the Lambda function
- **EventBridgeStack-RuleArn**: ARN of the EventBridge rule
- **EventBridgeStack-RuleName**: Name of the EventBridge rule

View outputs:
```bash
aws cloudformation describe-stacks \
  --stack-name InvertedYieldTraderProdLambdaStack \
  --query 'Stacks[0].Outputs'
```

## Monitoring Execution

Check Lambda execution logs:

```bash
# View recent logs
aws logs tail /aws/lambda/InvertedYieldTraderProd --follow

# Filter for errors
aws logs tail /aws/lambda/InvertedYieldTraderProd --follow --filter-pattern ERROR
```

Check EventBridge rule status:

```bash
aws events describe-rule \
  --name InvertedYieldTraderProdDailyTrigger \
  --region us-east-1
```

## Troubleshooting

### "FRED API key is required" Error

Provide API keys via context or environment variables. The app requires both FRED and Alpha Vantage keys at synthesis time.

### Lambda Function Not Executing

Check:
1. EventBridge rule is enabled: `aws events describe-rule --name InvertedYieldTraderProdDailyTrigger`
2. Lambda has permission from EventBridge: Check Lambda resource-based policy
3. Lambda function logs: `aws logs tail /aws/lambda/InvertedYieldTraderProd`

### S3 Access Denied

The Lambda execution role may not have S3 permissions. Check IAM role policies:

```bash
aws iam get-role-policy \
  --role-name inverted-yield-trader-prod-lambda-role \
  --policy-name DefaultPolicy
```

### DynamoDB Errors (if upgrading)

If upgrading to use DynamoDB instead of S3, update:
1. `src/LambdaMain.cpp` to write to DynamoDB
2. `iam-stack.ts` to grant DynamoDB permissions
3. Rebuild and redeploy Lambda binary

## Development

### Local TypeScript Compilation

```bash
npm run build
```

### Watch Mode

```bash
npx cdk watch
```

## Notes

- **Lambda Runtime**: Uses `provided.al2` runtime with custom binary
- **Timeout**: Set to 600 seconds (10 minutes) to allow data fetching from FRED and Alpha Vantage
- **Memory**: 512 MB allocated
- **Cost**: Minimal - daily Lambda invocation + S3 storage + EventBridge rule
- **Daily Execution**: EventBridge triggers Lambda every day at 12:00 UTC

## Security Considerations

1. **API Keys**: Passed as Lambda environment variables. Consider using AWS Secrets Manager for production.
2. **S3 Bucket**:
   - Public access blocked
   - Versioning enabled for audit trail
   - SSL enforced
   - S3-managed encryption enabled
3. **IAM Role**: Least-privilege principle - only S3 PutObject/GetObject on specific bucket
4. **Lambda Role**: Service-linked, cannot be assumed by other principals

## Next Steps

1. Deploy to development environment first to test
2. Verify Lambda execution in CloudWatch Logs
3. Check S3 bucket for daily analysis results
4. Deploy to production when satisfied
5. Consider adding SNS notifications for errors via EventBridge dead-letter queue
