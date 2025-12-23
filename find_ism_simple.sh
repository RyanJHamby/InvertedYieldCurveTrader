#!/bin/bash
# Find working ISM Manufacturing series on FRED

API_KEY="${FRED_API_KEY}"

echo "Testing ISM Manufacturing component indices..."
echo "=============================================="

test_series() {
    ID=$1
    NAME=$2
    RESPONSE=$(curl -s "https://api.stlouisfed.org/fred/series/observations?series_id=$ID&api_key=$API_KEY&file_type=json&limit=1&sort_order=desc")

    if echo "$RESPONSE" | grep -q '"observations"'; then
        VALUE=$(echo "$RESPONSE" | python3 -c "import sys, json; d=json.load(sys.stdin); obs=d['observations'][0]; print(f\"{obs['date']}: {obs['value']}\")" 2>/dev/null)
        echo "✅ $ID ($NAME): $VALUE"
        return 0
    else
        echo "❌ $ID ($NAME) not found"
        return 1
    fi
}

# Test each series
test_series "NAPMPI" "Production Index"
test_series "NAPMNOI" "New Orders Index"
test_series "NAPMEI" "Employment Index"
test_series "NAPMSDI" "Supplier Deliveries Index"
test_series "NAPMII" "Inventories Index"

echo ""
echo "=============================================="
echo "Recommendation: Use the series marked with ✅"
