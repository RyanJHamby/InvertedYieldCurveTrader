# Deployment Runbook: Production Setup

## Overview

This runbook walks through deploying the InvertedYieldCurveTrader to AWS Lambda with proper security, logging, and monitoring.

**Estimated time: 45 minutes**

---

## Prerequisites

- AWS account with appropriate permissions
- Docker installed locally
- `aws-cli` v2+
- `jq` for JSON parsing

Verify:
```bash
aws --version
docker --version
jq --version
```

---

## Phase 1: AWS Infrastructure Setup (10 minutes)

### Step 1.1: Create S3 Bucket for Results

```bash
BUCKET_NAME="inverted-yield-trader-daily-results"
AWS_REGION="us-east-1"

aws s3api create-bucket \
  --bucket $BUCKET_NAME \
  --region $AWS_REGION \
  --create-bucket-configuration LocationConstraint=$AWS_REGION

# Enable versioning (for audit trail)
aws s3api put-bucket-versioning \
  --bucket $BUCKET_NAME \
  --versioning-configuration Status=Enabled

# Enable encryption
aws s3api put-bucket-encryption \
  --bucket $BUCKET_NAME \
  --server-side-encryption-configuration '{
    "Rules": [{
      "ApplyServerSideEncryptionByDefault": {
        "SSEAlgorithm": "AES256"
      }
    }]
  }'

echo "✅ S3 bucket created: s3://$BUCKET_NAME"
```

### Step 1.2: Create IAM Role for Lambda

```bash
# Create role
ROLE_NAME="inverted-yield-trader-lambda-role"

aws iam create-role \
  --role-name $ROLE_NAME \
  --assume-role-policy-document '{
    "Version": "2012-10-17",
    "Statement": [{
      "Effect": "Allow",
      "Principal": {
        "Service": "lambda.amazonaws.com"
      },
      "Action": "sts:AssumeRole"
    }]
  }'

# Attach basic Lambda execution policy
aws iam attach-role-policy \
  --role-name $ROLE_NAME \
  --policy-arn arn:aws:iam::aws:policy/service-role/AWSLambdaBasicExecutionRole

# Create custom policy for S3 access
aws iam put-role-policy \
  --role-name $ROLE_NAME \
  --policy-name S3AccessPolicy \
  --policy-document '{
    "Version": "2012-10-17",
    "Statement": [{
      "Effect": "Allow",
      "Action": [
        "s3:PutObject",
        "s3:GetObject"
      ],
      "Resource": "arn:aws:s3:::'"$BUCKET_NAME"'/*"
    }]
  }'

# If using Secrets Manager (optional, for production):
# aws iam put-role-policy \
#   --role-name $ROLE_NAME \
#   --policy-name SecretsManagerPolicy \
#   --policy-document '{
#     "Version": "2012-10-17",
#     "Statement": [{
#       "Effect": "Allow",
#       "Action": "secretsmanager:GetSecretValue",
#       "Resource": "arn:aws:secretsmanager:us-east-1:ACCOUNT:secret:inverted-yield-trader/*"
#     }]
#   }'

# Get role ARN
ROLE_ARN=$(aws iam get-role --role-name $ROLE_NAME --query 'Role.Arn' --output text)
echo "✅ IAM role created: $ROLE_ARN"
```

### Step 1.3: Create Secrets in Secrets Manager (Production Only)

```bash
# Create secret for API keys (optional, recommended for prod)
aws secretsmanager create-secret \
  --name inverted-yield-trader/prod \
  --description "API keys for InvertedYieldCurveTrader" \
  --secret-string '{
    "fred_api_key": "your-fred-key-here",
    "alpha_vantage_api_key": "your-alpha-vantage-key-here"
  }'

echo "✅ Secret created in Secrets Manager"
echo "   Update the secret values: aws secretsmanager update-secret ..."
```

---

## Phase 2: Build & Package Lambda Function (15 minutes)

### Step 2.1: Build AWS SDK (one-time)

```bash
# Already built? Skip to Step 2.2

cd /tmp
git clone --recursive https://github.com/aws/aws-sdk-cpp.git
cd aws-sdk-cpp
mkdir build && cd build

cmake .. \
  -DBUILD_ONLY="s3" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="$HOME/Desktop/aws-sdk-cpp-install"

make -j4
make install

echo "✅ AWS SDK built and installed"
```

### Step 2.2: Build Lambda Docker Image

```bash
cd ~/Documents/InvertedYieldCurveTrader

# Copy AWS SDK to build context
cp -r ~/Desktop/aws-sdk-cpp-install ./

# Build image
docker build \
  -f Dockerfile.lambda \
  -t inverted-yield-trader:latest \
  .

echo "✅ Docker image built: inverted-yield-trader:latest"
```

### Step 2.3: Extract Binary from Docker Image

```bash
# Create temporary container
CONTAINER=$(docker create inverted-yield-trader:latest)

# Extract binary
docker cp $CONTAINER:/var/task/InvertedYieldCurveTrader ./lambda-binary

# Cleanup
docker rm $CONTAINER

# Verify binary
file ./lambda-binary
ls -lh ./lambda-binary

echo "✅ Lambda binary extracted"
```

### Step 2.4: Create Lambda Deployment Package

```bash
# Create working directory
WORK_DIR="/tmp/lambda-package"
mkdir -p $WORK_DIR

# Copy binary
cp ./lambda-binary $WORK_DIR/bootstrap
chmod +x $WORK_DIR/bootstrap

# Create wrapper script if needed
# (For now, binary is executable directly)

# Package it
cd $WORK_DIR
zip -r lambda-function.zip bootstrap

# Move to project root
mv lambda-function.zip ~/Documents/InvertedYieldCurveTrader/

echo "✅ Deployment package created: lambda-function.zip"
ls -lh ~/Documents/InvertedYieldCurveTrader/lambda-function.zip
```

---

## Phase 3: Create Lambda Function (10 minutes)

### Step 3.1: Deploy to Lambda

```bash
cd ~/Documents/InvertedYieldCurveTrader

FUNCTION_NAME="InvertedYieldTraderDaily"
RUNTIME="provided.al2"  # Use provided runtime for custom binaries

# Create function
aws lambda create-function \
  --function-name $FUNCTION_NAME \
  --runtime $RUNTIME \
  --role $ROLE_ARN \
  --handler bootstrap \
  --zip-file fileb://lambda-function.zip \
  --timeout 600 \
  --memory-size 512 \
  --environment Variables="{
    FRED_API_KEY=your-fred-api-key,
    ALPHA_VANTAGE_API_KEY=your-alpha-vantage-key,
    S3_BUCKET=$BUCKET_NAME
  }" \
  --region us-east-1

echo "✅ Lambda function created: $FUNCTION_NAME"

# Get function ARN
FUNCTION_ARN=$(aws lambda get-function \
  --function-name $FUNCTION_NAME \
  --query 'Configuration.FunctionArn' \
  --output text)

echo "   Function ARN: $FUNCTION_ARN"
```

### Step 3.2: Test Lambda Locally (Optional)

```bash
# Test with event
aws lambda invoke \
  --function-name $FUNCTION_NAME \
  --payload '{"mode": "covariance"}' \
  /tmp/lambda-response.json

# Check output
cat /tmp/lambda-response.json | jq .

# Check logs
aws logs tail /aws/lambda/$FUNCTION_NAME --follow
```

---

## Phase 4: Set Up Daily Trigger (10 minutes)

### Step 4.1: Create EventBridge Rule

```bash
# Create rule (daily at 12 UTC)
aws events put-rule \
  --name InvertedYieldTraderDailyTrigger \
  --schedule-expression "cron(0 12 * * ? *)" \
  --state ENABLED \
  --description "Daily trigger for InvertedYieldCurveTrader analysis"

echo "✅ EventBridge rule created"
```

### Step 4.2: Attach Lambda as Target

```bash
RULE_NAME="InvertedYieldTraderDailyTrigger"

# Add Lambda permission for EventBridge
aws lambda add-permission \
  --function-name $FUNCTION_NAME \
  --statement-id AllowEventBridgeInvoke \
  --action 'lambda:InvokeFunction' \
  --principal events.amazonaws.com \
  --source-arn "arn:aws:events:us-east-1:$(aws sts get-caller-identity --query Account --output text):rule/$RULE_NAME"

# Add Lambda as target
aws events put-targets \
  --rule $RULE_NAME \
  --targets "Id"="1","Arn"="$FUNCTION_ARN","RoleArn"="$(aws iam get-role --role-name service-role/EventBridgeInvokeLambdaRole --query 'Role.Arn' --output text 2>/dev/null || echo 'ROLE_TO_CREATE')"

echo "✅ Lambda attached as EventBridge target"
```

---

## Phase 5: Monitoring & Alerts (Optional, 5 minutes)

### Step 5.1: Create CloudWatch Dashboard

```bash
aws cloudwatch put-dashboard \
  --dashboard-name InvertedYieldTrader \
  --dashboard-body '{
    "widgets": [
      {
        "type": "metric",
        "properties": {
          "metrics": [
            ["AWS/Lambda", "Duration", {"stat": "Average"}],
            ["AWS/Lambda", "Errors", {"stat": "Sum"}],
            ["AWS/Lambda", "Invocations", {"stat": "Sum"}]
          ],
          "period": 300,
          "stat": "Average",
          "region": "us-east-1",
          "title": "Lambda Metrics"
        }
      }
    ]
  }'

echo "✅ CloudWatch dashboard created"
```

### Step 5.2: Create Alarms

```bash
# Alert on Lambda errors
aws cloudwatch put-metric-alarm \
  --alarm-name InvertedYieldTrader-HighErrorRate \
  --alarm-description "Alert if Lambda errors exceed 1 per day" \
  --metric-name Errors \
  --namespace AWS/Lambda \
  --statistic Sum \
  --period 86400 \
  --threshold 1 \
  --comparison-operator GreaterThanOrEqualToThreshold \
  --evaluation-periods 1 \
  --dimensions Name=FunctionName,Value=$FUNCTION_NAME

# Alert on Lambda timeout
aws cloudwatch put-metric-alarm \
  --alarm-name InvertedYieldTrader-Timeout \
  --alarm-description "Alert if Lambda times out" \
  --metric-name Duration \
  --namespace AWS/Lambda \
  --statistic Maximum \
  --period 300 \
  --threshold 550000 \
  --comparison-operator GreaterThanThreshold \
  --evaluation-periods 1 \
  --dimensions Name=FunctionName,Value=$FUNCTION_NAME

echo "✅ CloudWatch alarms created"
```

---

## Phase 6: Validation (5 minutes)

### Step 6.1: Verify Daily Run

```bash
# Wait for 12 UTC or trigger manually for testing
aws lambda invoke \
  --function-name $FUNCTION_NAME \
  /tmp/test-response.json

# Check response
cat /tmp/test-response.json

# Check logs
aws logs tail /aws/lambda/$FUNCTION_NAME --follow --since 1m

# Check S3 upload
aws s3 ls s3://$BUCKET_NAME/positions/ --human-readable --recursive
```

### Step 6.2: Verify Data in S3

```bash
# Download latest result
LATEST=$(aws s3 ls s3://$BUCKET_NAME/positions/ | tail -1 | awk '{print $NF}')
aws s3 cp s3://$BUCKET_NAME/positions/$LATEST ./today-result.json

# Inspect
jq . ./today-result.json | head -50
```

---

## Post-Deployment Checklist

- [ ] S3 bucket created and versioned
- [ ] IAM role with S3 access
- [ ] Lambda function deployed
- [ ] EventBridge rule created and attached
- [ ] Daily trigger scheduled (12 UTC)
- [ ] Test run successful with data in S3
- [ ] CloudWatch logs show successful execution
- [ ] Alarms configured
- [ ] Monitoring dashboard accessible

---

## Troubleshooting

### Lambda Timeout
```bash
# Increase timeout
aws lambda update-function-configuration \
  --function-name $FUNCTION_NAME \
  --timeout 900  # 15 minutes
```

### Check Latest Logs
```bash
aws logs tail /aws/lambda/$FUNCTION_NAME --follow --since 30m
```

### View Lambda Errors
```bash
aws lambda get-function-concurrency --function-name $FUNCTION_NAME

# If error rate is high:
aws cloudwatch get-metric-statistics \
  --namespace AWS/Lambda \
  --metric-name Errors \
  --dimensions Name=FunctionName,Value=$FUNCTION_NAME \
  --start-time $(date -u -d '1 hour ago' +%Y-%m-%dT%H:%M:%S) \
  --end-time $(date -u +%Y-%m-%dT%H:%M:%S) \
  --period 300 \
  --statistics Sum
```

### Test API Keys
```bash
# Verify FRED API works
curl -s "https://api.stlouisfed.org/fred/series/UNRATE/observations?api_key=$FRED_API_KEY&file_type=json" | jq '.observations | length'

# Verify Alpha Vantage works
curl -s "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=SPY&apikey=$ALPHA_VANTAGE_API_KEY" | jq '.Global Quote'
```

---

## Costs

| Service | Daily Usage | Monthly Cost |
|---------|------------|--------------|
| Lambda (600 sec × 512 MB) | 5 GB-seconds | $0.08 |
| S3 storage (30 files) | 150 KB | $0.05 |
| CloudWatch Logs | 100 KB/day | $0.05 |
| EventBridge | 1 rule | free |
| **Total** | | **~$0.20** |

---

## Next Steps

1. ✅ Deploy to Lambda
2. ⏳ Monitor for 30 days
3. ⏳ Validate position sizing recommendations against ES prices
4. ⏳ Add historical backtesting queries (Athena + S3)
5. ⏳ Set up alerts for regime changes
6. ⏳ Consider SNS topic for daily summaries

---

## Support

If deployment fails:
1. Check Lambda logs: `aws logs tail /aws/lambda/InvertedYieldTraderDaily`
2. Verify API keys work: `curl https://api.stlouisfed.org/fred/series/...`
3. Test binary locally: `./InvertedYieldCurveTrader covariance`
4. Review CloudWatch Logs Insights: `aws logs start-query`
