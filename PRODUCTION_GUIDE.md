# Production Deployment Guide

## Quick Summary: What Was Added

### Security
- ✅ **SecretsManager**: Reads API keys from AWS Secrets Manager (or env vars for dev)
- ✅ **Structured Logging**: JSON format for CloudWatch integration
- ✅ **Error Handling**: Try-catch blocks with context-aware logging
- ✅ **IAM Roles**: Least-privilege S3 access

### Infrastructure
- ✅ **Docker Build**: Dockerfile.lambda for Lambda-compatible binary
- ✅ **S3 Storage**: JSON results with versioning and encryption
- ✅ **EventBridge**: Daily 12 UTC automated trigger
- ✅ **CloudWatch**: Structured JSON logs + dashboards + alarms

---

## Files Added/Modified

### New Files
```
src/Utils/SecretsManager.hpp/cpp    # Secure credential management
src/Utils/Logger.hpp/cpp             # Structured JSON logging
Dockerfile.lambda                    # Build for Lambda
DEPLOYMENT_RUNBOOK.md                # Step-by-step deployment
PRODUCTION_GUIDE.md                  # This file
```

### Modified Files
```
src/CMakeLists.txt                   # Removed dynamodb, kept s3
src/MultiVarAnalysisWorkflow.cpp     # Added logging, secrets, error handling
```

---

## Architecture

```
┌─────────────────────────────────────────┐
│   EventBridge (Daily 12 UTC)            │
└────────────────┬────────────────────────┘
                 │
                 ↓
┌─────────────────────────────────────────┐
│   Lambda Function                       │
│  ├─ SecretsManager::getAllSecrets()     │
│  ├─ Logger::info() → CloudWatch Logs    │
│  ├─ Run: covariance analysis            │
│  ├─ Write: output.txt                   │
│  └─ Upload: S3 → positions/YYYY-MM-DD   │
└─────────────────┬───────────────────────┘
                  │
                  ↓
     ┌───────────────────────────┐
     │ S3 Bucket (Encrypted)     │
     │ positions/                │
     │  ├─ 2025-12-25.json       │
     │  ├─ 2025-12-26.json       │
     │  └─ ...                   │
     └───────────────────────────┘
                  │
                  ↓
     ┌───────────────────────────┐
     │ CloudWatch               │
     │ ├─ Logs (JSON)           │
     │ ├─ Metrics               │
     │ └─ Alarms                │
     └───────────────────────────┘
```

---

## Deployment Path

### Option A: Quick Start (30 minutes)

```bash
# 1. Build AWS SDK (one-time, 10 min)
cd /tmp
git clone --recursive https://github.com/aws/aws-sdk-cpp.git
cd aws-sdk-cpp && mkdir build && cd build
cmake .. -DBUILD_ONLY="s3" -DCMAKE_INSTALL_PREFIX=$HOME/Desktop/aws-sdk-cpp-install
make -j4 && make install

# 2. Build Docker image
cd ~/Documents/InvertedYieldCurveTrader
cp -r ~/Desktop/aws-sdk-cpp-install ./
docker build -f Dockerfile.lambda -t inverted-yield-trader:latest .

# 3. Extract binary
CONTAINER=$(docker create inverted-yield-trader:latest)
docker cp $CONTAINER:/var/task/InvertedYieldCurveTrader ./lambda-binary
docker rm $CONTAINER

# 4. Create deployment package
mkdir -p /tmp/lambda && cp ./lambda-binary /tmp/lambda/bootstrap
cd /tmp/lambda && zip -r lambda-function.zip bootstrap && mv lambda-function.zip ~/Documents/InvertedYieldCurveTrader/

# 5. Deploy to Lambda
aws lambda create-function \
  --function-name InvertedYieldTraderDaily \
  --runtime provided.al2 \
  --role arn:aws:iam::YOUR_ACCOUNT:role/inverted-yield-trader-lambda-role \
  --handler bootstrap \
  --zip-file fileb://lambda-function.zip \
  --timeout 600 \
  --memory-size 512 \
  --environment Variables="{FRED_API_KEY=your_key,ALPHA_VANTAGE_API_KEY=your_key,S3_BUCKET=inverted-yield-trader-daily-results}"
```

**See DEPLOYMENT_RUNBOOK.md for complete step-by-step instructions.**

---

## Key Features

### 1. Secure Credential Management

**Development (local):**
```bash
export FRED_API_KEY="your-key"
export ALPHA_VANTAGE_API_KEY="your-key"
./InvertedYieldCurveTrader covariance
```

**Production (Lambda):**
- Keys stored in AWS Secrets Manager
- Never appears in logs or CloudWatch
- Code automatically detects environment and fetches accordingly

Implementation:
```cpp
std::map<std::string, std::string> secrets = SecretsManager::getAllSecrets();
std::string fredKey = secrets["fred_api_key"];
```

### 2. Structured JSON Logging

Every log entry is valid JSON with context:

```json
{
  "level": "INFO",
  "timestamp": "2025-12-25T12:30:45.123Z",
  "message": "Data fetch successful",
  "indicators_fetched": 8
}
```

Usage:
```cpp
Logger::info("Data fetch successful", {
    {"indicators_fetched", rawData.size()}
});
```

Benefits:
- ✅ CloudWatch Logs Insights can query with SQL
- ✅ Easy to grep/filter in logs
- ✅ Structured metrics without extra instrumentation
- ✅ Stack traces include exception context

### 3. Error Handling with Context

```cpp
try {
    secrets = SecretsManager::getAllSecrets();
} catch (const std::exception& e) {
    Logger::critical("Failed to retrieve API keys", e);
    return 1;
}
```

Produces:
```json
{
  "level": "CRITICAL",
  "timestamp": "2025-12-25T12:30:46.456Z",
  "message": "Failed to retrieve API keys",
  "exception": "Environment variable 'FRED_API_KEY' not set"
}
```

### 4. CloudWatch Integration

Logs are automatically captured. View with:

```bash
# Real-time monitoring
aws logs tail /aws/lambda/InvertedYieldTraderDaily --follow

# Search for errors
aws logs filter-log-events \
  --log-group-name /aws/lambda/InvertedYieldTraderDaily \
  --filter-pattern '"level":"ERROR"'

# Query with Insights (SQL-like)
aws logs start-query \
  --log-group-name /aws/lambda/InvertedYieldTraderDaily \
  --start-time $(date -d '1 hour ago' +%s) \
  --end-time $(date +%s) \
  --query-string 'fields @timestamp, level, message | stats count() by level'
```

---

## Testing Locally

### Before Deploying

```bash
# 1. Set environment variables
export FRED_API_KEY="your-key"
export ALPHA_VANTAGE_API_KEY="your-key"

# 2. Run covariance mode
./InvertedYieldCurveTrader covariance

# Should produce:
# ✓ output.txt with factor results
# ✓ JSON logs to stdout

# 3. Check output
cat output.txt

# 4. Test S3 upload (if AWS credentials configured)
./InvertedYieldCurveTrader upload-daily
```

### Verify Setup

```bash
# Test FRED API
curl -s "https://api.stlouisfed.org/fred/series/UNRATE/observations?api_key=$FRED_API_KEY" | jq '.observations | length'

# Test Alpha Vantage
curl -s "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=SPY&apikey=$ALPHA_VANTAGE_API_KEY" | jq '.Global Quote'

# Test AWS credentials
aws sts get-caller-identity

# Test S3 access
aws s3 ls inverted-yield-trader-daily-results/
```

---

## Monitoring & Alerts

### CloudWatch Dashboard

Automatically created in DEPLOYMENT_RUNBOOK shows:
- Lambda invocations (daily count)
- Duration (execution time)
- Error rate
- Log output

### Metrics to Watch

| Metric | Alert Threshold | Action |
|--------|-----------------|--------|
| Errors | > 0/day | Check logs, restart |
| Duration | > 550 sec | Increase timeout |
| Invocations | 0 in 24h | Check EventBridge rule |
| S3 uploads | 0 in 24h | Check Lambda error logs |

### Example Alert

```bash
aws cloudwatch put-metric-alarm \
  --alarm-name InvertedYieldTrader-NoDataInS3 \
  --alarm-description "Alert if no data uploaded in 24h" \
  --metric-name NumberOfObjects \
  --namespace AWS/S3 \
  --statistic Sum \
  --period 86400 \
  --threshold 1 \
  --comparison-operator LessThanThreshold
```

---

## Cost Breakdown

**Monthly estimate (30 runs):**

| Service | Cost |
|---------|------|
| Lambda (600 sec × 512MB) | $0.08 |
| S3 storage (30 files × 5KB) | $0.05 |
| CloudWatch Logs | $0.05 |
| Data transfer (S3 → Lambda) | free |
| **Total** | **$0.18** |

**Annual: ~$2.20**

---

## Troubleshooting Checklist

### "Lambda timeout"
```bash
aws lambda update-function-configuration \
  --function-name InvertedYieldTraderDaily \
  --timeout 900  # 15 minutes
```

### "API key not found"
```bash
# Verify env vars are set
aws lambda get-function-configuration \
  --function-name InvertedYieldTraderDaily | jq '.Environment'

# Update if needed
aws lambda update-function-configuration \
  --function-name InvertedYieldTraderDaily \
  --environment Variables="{FRED_API_KEY=new-key,...}"
```

### "S3 upload failed"
```bash
# Check IAM permissions
aws iam get-role-policy \
  --role-name inverted-yield-trader-lambda-role \
  --policy-name S3AccessPolicy

# Check S3 bucket exists and is accessible
aws s3 ls inverted-yield-trader-daily-results/
```

### "No data in CloudWatch Logs"
```bash
# Manually invoke Lambda to see logs
aws lambda invoke \
  --function-name InvertedYieldTraderDaily \
  /tmp/response.json

# Check logs
aws logs tail /aws/lambda/InvertedYieldTraderDaily
```

---

## Next: After 30 Days of Deployment

1. **Analyze Position Sizing**
   - Extract S3 positions: `aws s3 sync s3://bucket/positions/ ./`
   - Compare with ES prices from that day
   - Did sizing reduce volatility?

2. **Add Backtesting**
   - Query historical S3 data with Athena
   - Replay 2008, 2020, 2022
   - Validate factor attribution is correct

3. **Enhance Monitoring**
   - Add SNS topic for daily summaries
   - Create Slack notifications for anomalies
   - Set up cost alarms (if spending > $0.50/month)

4. **Optimize Performance**
   - Profile Lambda execution time
   - Cache frequently-accessed FRED data
   - Consider parallel data fetching

---

## References

- [AWS Lambda Custom Runtimes](https://docs.aws.amazon.com/lambda/latest/dg/runtimes-custom.html)
- [CloudWatch Logs Insights Queries](https://docs.aws.amazon.com/AmazonCloudWatch/latest/logs/CWL_QuerySyntax.html)
- [S3 Best Practices](https://docs.aws.amazon.com/AmazonS3/latest/userguide/BestPractices.html)
- [IAM Best Practices](https://docs.aws.amazon.com/IAM/latest/userguide/best-practices.html)

---

## Summary: You're Ready to Deploy

This production setup includes:
- ✅ Secure credential management
- ✅ Structured logging for monitoring
- ✅ Error handling with context
- ✅ Docker build for Lambda
- ✅ CloudWatch integration
- ✅ Step-by-step deployment guide

**Next step: Follow DEPLOYMENT_RUNBOOK.md**
