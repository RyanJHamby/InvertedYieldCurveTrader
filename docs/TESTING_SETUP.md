# Testing Setup Guide

## Prerequisites

Install Python testing dependencies:

```bash
pip install pytest pytest-cov requests boto3
```

## Running Unit Tests

### Option 1: Run All Tests
```bash
# From project root
pytest tests/ -v
```

### Option 2: Run Specific Test File
```bash
# Test FRED API client only
pytest tests/test_fred_api_client.py -v
```

### Option 3: Run with Coverage Report
```bash
# See which lines are tested
pytest tests/test_fred_api_client.py -v --cov=src/Lambda --cov-report=html

# Open coverage report in browser
open htmlcov/index.html
```

### Option 4: Run Integration Tests (requires real API keys)
```bash
# Set API key first
export FRED_API_KEY="your_actual_api_key"

# Run integration tests only
pytest tests/test_fred_api_client.py -v -m integration

# Or run all tests including integration
pytest tests/test_fred_api_client.py -v
```

## Test Output Example

```
tests/test_fred_api_client.py::TestFREDAPIClientInitialization::test_init_with_valid_api_key PASSED
tests/test_fred_api_client.py::TestFREDAPIClientInitialization::test_init_with_none_api_key PASSED
tests/test_fred_api_client.py::TestFREDAPIClientFetchSeries::test_fetch_series_success PASSED
...
==================== 32 passed in 0.87s ====================
```

## Running Tests Automatically

### Pre-commit Hook (Optional)
```bash
# Create .git/hooks/pre-commit
cat > .git/hooks/pre-commit << 'EOF'
#!/bin/bash
pytest tests/ -v --tb=short
if [ $? -ne 0 ]; then
    echo "Tests failed. Commit aborted."
    exit 1
fi
EOF

chmod +x .git/hooks/pre-commit
```

### GitHub Actions (Optional)
Create `.github/workflows/test.yml`:
```yaml
name: Tests
on: [push, pull_request]
jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - uses: actions/setup-python@v2
        with:
          python-version: '3.10'
      - run: pip install pytest pytest-cov requests boto3
      - run: pytest tests/ -v --cov
```

## Test Coverage Goals

- **Unit tests:** 100% coverage (mock all external calls)
- **Integration tests:** Optional (require real API keys)
- **Every new feature:** Must have tests before merging

## Debugging Failed Tests

### Verbose output:
```bash
pytest tests/test_fred_api_client.py -v -s
```

### Stop on first failure:
```bash
pytest tests/test_fred_api_client.py -x
```

### Run specific test:
```bash
pytest tests/test_fred_api_client.py::TestFREDAPIClientInitialization::test_init_with_valid_api_key -v
```

### Enter debugger on failure:
```bash
pytest tests/test_fred_api_client.py --pdb
```
