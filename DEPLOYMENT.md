# Deployment Guide: Cloud Architecture (S3-Based)

## Overview

The refactored system uses **S3 for persistence** instead of DynamoDB. This is cheaper, simpler, and more appropriate for daily batch analysis.

**Cost:** ~$0.50/month (vs $2-5/month for DynamoDB)

---

## Architecture

```
┌─────────────────────────────────────────────────────┐
│  Daily Workflow (Lambda / EventBridge)              │
└─────────────────────────────────────────────────────┘
                        ↓
                   Step 1: Run
    ./InvertedYieldCurveTrader covariance
           (FRED + Alpha Vantage APIs)
                        ↓
            Generates: output.txt
            (factor results + position sizing)
                        ↓
                   Step 2: Upload
    ./InvertedYieldCurveTrader upload-daily
           (Write to S3 bucket)
                        ↓
    s3://inverted-yield-trader-daily-results/
        positions/2025-12-25.json
        positions/2025-12-26.json
        ...
```

---

## Prerequisites

### 1. AWS Account Setup

Create an S3 bucket for results:

```bash
aws s3api create-bucket \
  --bucket inverted-yield-trader-daily-results \
  --region us-east-1
```

### 2. Lambda IAM Role

Create an IAM role with permissions:

```json
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Action": "s3:PutObject",
      "Resource": "arn:aws:s3:::inverted-yield-trader-daily-results/*"
    }
  ]
}
```

### 3. Environment Variables

Set these in Lambda configuration:
- `FRED_API_KEY`: Your FRED API key
- `ALPHA_VANTAGE_API_KEY`: Your Alpha Vantage key
- `S3_BUCKET`: `inverted-yield-trader-daily-results` (optional, uses default if not set)

---

## Local Testing

### Step 1: Run Analysis

```bash
cd ~/Documents/InvertedYieldCurveTrader
./InvertedYieldCurveTrader covariance
```

**Output:**
- Prints factor decomposition to stdout
- Writes `output.txt` with:
  - Regime volatility (Frobenius norm)
  - Factor labels + confidence
  - Position sizing recommendations

### Step 2: Upload to S3 (Local Test)

First, configure AWS credentials:

```bash
aws configure
# Enter: Access Key, Secret Key, Region (us-east-1)
```

Then upload:

```bash
./InvertedYieldCurveTrader upload-daily
```

**Success message:**
```
✅ Successfully uploaded to s3://inverted-yield-trader-daily-results/positions/2025-12-25.json
```

### Step 3: Verify in S3

```bash
aws s3 ls s3://inverted-yield-trader-daily-results/positions/ --human-readable
```

---

## AWS Lambda Deployment

### 1. Build Binary

On a macOS machine with same architecture as Lambda (ARM64):

```bash
cd ~/Documents/InvertedYieldCurveTrader/src
mkdir -p build && cd build
cmake ..
make -j4

# Binary location:
# /path/to/InvertedYieldCurveTrader
```

### 2. Create Lambda Function

```bash
# Package the binary
zip function.zip InvertedYieldCurveTrader

# Create Lambda function
aws lambda create-function \
  --function-name InvertedYieldTraderDaily \
  --runtime provided.al2 \
  --role arn:aws:iam::YOUR_ACCOUNT:role/lambda-s3-role \
  --handler bootstrap \
  --zip-file fileb://function.zip \
  --environment "Variables={FRED_API_KEY=your_key,ALPHA_VANTAGE_API_KEY=your_key}" \
  --timeout 300 \
  --memory-size 512
```

### 3. Set Up EventBridge Trigger

```bash
# Daily at 12 UTC
aws events put-rule \
  --name InvertedYieldTraderDailyTrigger \
  --schedule-expression "cron(0 12 * * ? *)" \
  --state ENABLED

# Target the Lambda
aws events put-targets \
  --rule InvertedYieldTraderDailyTrigger \
  --targets "Id"="1","Arn"="arn:aws:lambda:us-east-1:YOUR_ACCOUNT:function:InvertedYieldTraderDaily"
```

---

## Monitoring

### Check Daily Uploads

```bash
# List all positions
aws s3 ls s3://inverted-yield-trader-daily-results/positions/

# Download today's result
aws s3 cp s3://inverted-yield-trader-daily-results/positions/2025-12-25.json ./today.json
cat today.json | jq .
```

### Monitor Lambda Logs

```bash
# View CloudWatch logs
aws logs tail /aws/lambda/InvertedYieldTraderDaily --follow
```

### Set Alarms

```bash
# Alert if Lambda fails more than 2x in a day
aws cloudwatch put-metric-alarm \
  --alarm-name InvertedYieldTrader-HighErrorRate \
  --alarm-description "Alert if Lambda errors exceed threshold" \
  --metric-name Errors \
  --namespace AWS/Lambda \
  --statistic Sum \
  --period 86400 \
  --threshold 2 \
  --comparison-operator GreaterThanThreshold \
  --evaluation-periods 1
```

---

## What Gets Stored

Each daily file (`positions/YYYY-MM-DD.json`) contains:

```json
{
  "date": "2025-12-25",
  "timestamp": 1735123200,
  "output": "
    Factor 1: Growth
      Variance: 0.40
      Confidence: 85.0%
      ...
    Portfolio Total Risk (Daily Vol): 0.15
    ...
    Position Sizing Recommendation:
      Recommended Notional: $4750000
      Recommended Shares (ES contracts): 950
    ..."
}
```

---

## Querying Historical Data

To analyze trends over time:

```bash
# Download all files
aws s3 sync s3://inverted-yield-trader-daily-results/positions/ ./positions/

# Parse with jq (e.g., extract position sizes)
for f in positions/*.json; do
  jq '.output' "$f" | grep "Recommended Notional"
done
```

Or use **Athena** if you want SQL queries:

```bash
# Create external table on S3 JSON files
CREATE EXTERNAL TABLE positions (
  date STRING,
  timestamp BIGINT,
  output STRING
)
STORED AS JSON
LOCATION 's3://inverted-yield-trader-daily-results/positions/'
```

---

## Cost Breakdown

| Service | Daily Usage | Monthly Cost |
|---------|------------|--------------|
| S3 (1 file/day) | 30 files × 5 KB | $0.23 |
| Lambda (300 sec × 512 MB) | 5 GB-seconds | $0.08 |
| CloudWatch Logs | ~100 KB/day | $0.05 |
| **Total** | | **~$0.40** |

---

## Troubleshooting

### Lambda Timeout

If you see `RequestId: ... Process exited before completing request`, increase timeout:

```bash
aws lambda update-function-configuration \
  --function-name InvertedYieldTraderDaily \
  --timeout 600  # 10 minutes
```

### S3 Upload Fails

Check:
1. IAM role has `s3:PutObject` on bucket
2. Bucket name in `S3_BUCKET` env var matches actual bucket
3. FRED/Alpha Vantage keys are valid

### Missing Data in Positions File

Ensure output.txt was generated:
```bash
ls -la output.txt
cat output.txt
```

If missing, Lambda didn't complete the covariance step. Check CloudWatch logs.

---

## Next Steps

1. ✅ Deploy Lambda function
2. ✅ Set up EventBridge trigger
3. ⏳ Let it run for 30 days
4. ⏳ Analyze position sizing recommendations
5. ⏳ Validate against actual ES prices
6. ⏳ Consider adding Athena queries for historical analysis

---

## Costs vs DynamoDB (Old)

| Metric | DynamoDB | S3 |
|--------|----------|-----|
| Daily write (1 item) | $0.0008 | $0.00002 |
| Monthly (30 writes) | $0.024 | $0.0006 |
| Storage (1 year of daily) | $0.50+ | $0.01 |
| Query cost | High (per scan) | Free (S3 listing) |
| Operational complexity | High (schema, indexes) | Low (just files) |

**Savings:** ~95% cheaper, much simpler.

---

## References

- [AWS Lambda Custom Runtime](https://docs.aws.amazon.com/lambda/latest/dg/runtimes-custom.html)
- [EventBridge Rules](https://docs.aws.amazon.com/eventbridge/latest/userguide/eb-create-rule.html)
- [S3 Cost Optimization](https://aws.amazon.com/s3/pricing/)
