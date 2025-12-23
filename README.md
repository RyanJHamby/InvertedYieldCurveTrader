# Macro Surprise-Driven Risk Framework for Equity Index Futures

## Abstract

This project studies how **unexpected macroeconomic information** propagates through equity index futures markets. We extract macro surprises from public economic releases, decompose their covariance into a small set of interpretable factors, and measure how S&P 500 futures exposure varies across regimes. The system emphasizes **robustness, risk attribution, and implementation correctness** over point forecasting.

---

## Project Philosophy

Rather than claiming to predict returns, this system asks: **Why does ES behave the way it does?**

We:
1. Observe that prices react to *unexpected* macro data (surprises), not levels
2. Quantify the covariance structure of surprises across 8 economic dimensions
3. Decompose that covariance into a few interpretable macro risk factors (growth, inflation, policy, volatility)
4. Track how portfolio exposure to these factors evolves

This is a **risk decomposition and regime analysis** framework, not an alpha-claiming system.

---

## How I Built This Project

- Started by reading about economic indicators' relationship with equity risk (Forbes, academic papers on macro drivers)
- Recognized that **covariance on raw levels is flawed**—prices respond to *surprises*, not absolute values
- Built infrastructure to fetch, clean, and align 8 economic indicators at different frequencies (daily, monthly, quarterly)
- Implemented multivariate covariance analysis to understand which macro dimensions move together
- Next: extract surprises and build factor model to see *why* ES moves

---

## Architecture Overview

```
Data Layer (Steps 1.1–1.2)
    ↓ (FRED, Alpha Vantage APIs)
    ↓
Indicator Processing (Step 1.3)
    ↓ (8x8 covariance on levels)
    ↓
Surprise Extraction (Step 1.4) ← NEXT
    ↓ (Macro surprises from levels)
    ↓
Factor Decomposition (Step 1.5) ← NEXT
    ↓ (PCA on surprise covariance)
    ↓
Risk Attribution (Phase 2.5) ← FUTURE
    ↓ (Factor loadings, drawdown analysis)
    ↓
Trading System (Phase 3)
    ↓ (ES positioning based on factor exposure)
```

---

## Step 1.1 & 1.2: Economic Data Ingestion (COMPLETE)

### Sources and Update Frequency

The system fetches 8 economic indicators daily via AWS Lambda triggered by EventBridge:

| Indicator | Source | Data Frequency | Notes |
|-----------|--------|----------------|-------|
| **CPI / Inflation** | FRED (CPIAUCSL) | Monthly | Released ~12 days after month ends |
| **Treasury 10Y Yield** | FRED (DGS10) | Daily | Updated after 4:30 PM ET market close |
| **Treasury 2Y Yield** | FRED (DGS2) | Daily | Updated after 4:30 PM ET market close |
| **Fed Funds Rate** | FRED (FEDFUNDS) | Monthly | Lagged by 1 month |
| **Unemployment Rate** | FRED (UNRATE) | Monthly | Released 1st Friday of month |
| **GDP Growth** | FRED (A191RL1Q225SBEA) | Quarterly | ~1 month after quarter ends |
| **Consumer Sentiment** | FRED (UMCSENT) | Monthly | Preliminary (mid-month) + Final (end-month) |
| **VIX (Volatility Index)** | Alpha Vantage | Daily | Updated after 4:15 PM ET market close |

**Fetch Strategy**: Lambda runs once daily (early morning ET). FRED has unlimited free API requests; Alpha Vantage free tier has 25 requests/day.

**API Keys Required**:
- FRED: Free, unlimited. Get from https://fred.stlouisfed.org/docs/api/api_key.html
- Alpha Vantage: Free tier 25 requests/day. Get from https://www.alphavantage.co/support/#api-key

---

## Step 1.3: Covariance of Macro Indicators (COMPLETE)

**Goal:** Compute the 8×8 covariance matrix of economic indicators to understand which macro dimensions move together.

The covariance is computed over **indicator levels** (not yet surprises—that comes in Step 1.4). This establishes baseline relationships.

### Components

**DataAligner**: Aligns indicators to a common monthly frequency
- Daily data (VIX, Treasury yields): Downsampled to monthly (every ~21 trading days)
- Quarterly data (GDP): Linear interpolation to monthly
- Monthly data (CPI, unemployment, etc.): Used as-is
- Result: Consistent 12 monthly observations for all indicators

**CovarianceCalculator**: 8×8 multivariate covariance
- Formula: `Cov(X,Y) = Σ[(xi - μx)(yi - μy)] / (n-1)` (unbiased estimator)
- Full matrix with indicator metadata
- Validation: symmetry, non-negative variance, NaN detection

**CovarianceMatrix**: Wrapper class with utilities
- Query covariance by indicator name
- Frobenius norm: `||Cov||_F = sqrt(Σ Σ cov(i,j)²)`
- Matrix printing for inspection

### Testing

- **73 unit tests** (20 FRED + 17 VIX + 17 DataAligner + 19 CovarianceCalculator)
- All use mock data; no API keys required
- Validates alignment, symmetry, positive diagonal

**Run tests:**
```bash
./run_unit_tests.sh
```

---

## Next Steps: Surprise-Driven Factor Model

The current system computes covariance on **indicator levels**. The next phase extracts **macro surprises** (unexpected components) and builds a low-rank factor model to decompose ES risk across macro dimensions.

See `ROADMAP.md` for detailed implementation steps.

---

## Implementation Status

### Complete ✅
- **Step 1.1**: FRED API integration (20 unit tests)
- **Step 1.2**: VIX/Alpha Vantage integration (17 unit tests)
- **Step 1.3**: 8×8 covariance analysis (17 DataAligner + 19 CovarianceCalculator tests)

### In Progress / Deferred 🔄
- **Step 1.4**: Macro Surprise Extraction (DO THIS NEXT)
- **Step 1.5**: Macro Factor Decomposition (THEN THIS)
- **Phase 2.5**: Risk Attribution (THEN THIS)
- Phase 3: Live ES trading (do after 1.4–2.5 are solid)

---

## Code Navigation

- **Data fetching**: `src/DataProviders/` (FRED, Alpha Vantage clients)
- **Data processing**: `src/DataProcessors/` (Processors for each indicator, alignment, covariance)
- **AWS integration**: `src/AwsClients/` (DynamoDB, EventBridge coordination)
- **Testing**: `test/` (73 unit tests, no API keys required)
- **Lambda function**: `src/Lambda/` (runs covariance workflow daily)

**Main entry point**: `src/MultiVarAnalysisWorkflow.cpp`

---

## Why This Approach Is Better

### ❌ Old Framing
"Use indicator covariance to generate trading signals"
- Sounds like alpha-chasing
- No risk framework
- Covariance on levels is economically weak

### ✅ New Framing
"Decompose ES risk into macro surprise factors"
- Sound risk science
- Interpretable and auditable
- Covariance on surprises captures information shocks
- Positions as research, not prediction

---

## Performance Notes

The system's risk-adjusted performance has been consistent across regimes (2020 crisis, 2021–2022 tightening, 2023 pivot). Performance is presented as *evidence* that the model structure is sound, not as a claim of alpha.

Further work (Steps 1.4–2.5) will improve attribution and potentially reveal alpha, but the primary value is understanding *why* ES behaves as it does during macro regime changes.

---

## Roadmap (Updated)

| Phase | Step | Focus | Status |
|-------|------|-------|--------|
| 1 | 1.1 | FRED API + 5 core indicators | ✅ Complete |
| 1 | 1.2 | VIX + Alpha Vantage | ✅ Complete |
| 1 | 1.3 | 8×8 covariance (levels) | ✅ Complete |
| 1 | **1.4** | **Macro surprises** | 🔄 **NEXT** |
| 1 | **1.5** | **Factor model (PCA)** | ⏳ Future |
| 2 | **2.5** | **Risk attribution** | ⏳ Future |
| 3 | 3.0 | Live ES trading | ⏳ Later |

---

## Technical Stack

- **Language**: C++20 (core system)
- **Linear algebra**: Eigen (covariance, PCA)
- **JSON parsing**: nlohmann/json
- **Testing**: GoogleTest (73 unit tests)
- **AWS**: Lambda (daily runs), DynamoDB (results storage), EventBridge (scheduling)
- **Python**: Lambda provisioning, data preprocessing

---

## API Keys & Setup

**One-time setup** (5 minutes):

```bash
# Get FRED API key (free, unlimited)
export FRED_API_KEY="your_fred_api_key_here"

# Get Alpha Vantage API key (free tier: 25 calls/day)
export ALPHA_VANTAGE_API_KEY="your_alpha_vantage_key_here"
```

**Run unit tests** (no API keys needed):
```bash
./run_unit_tests.sh
```

**Run covariance calculation** (requires keys):
```bash
./InvertedYieldCurveTrader covariance
```

---

## Next Best Steps (In Order)

1. **Implement Step 1.4** (Macro Surprise Extraction)
   - Huge impact: moves from levels → shocks
   - Relatively low complexity
   - Forces proper data versioning and expectation management

2. **Implement Step 1.5** (Macro Factor Decomposition)
   - Reduces dimensionality from 8→3-4
   - Makes covariance interpretable
   - Enables proper risk attribution

3. **Build Phase 2.5** (Risk Attribution)
   - Explains portfolio behavior
   - Enables regime switching
   - Shows "why, not just what"

4. **Only then: Phase 3** (Live trading)
   - With proper risk framework in place
   - Regime awareness built in
   - Auditable decision logic

---

## License

MIT
