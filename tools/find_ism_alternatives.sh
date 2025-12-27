#!/bin/bash
# Find working ISM Manufacturing series on FRED

API_KEY="${FRED_API_KEY}"

echo "Testing ISM Manufacturing component indices..."
echo "=============================================="

# Component indices that might be available
declare -A SERIES=(
    ["NAPMPI"]="Production Index"
    ["NAPMNOI"]="New Orders Index"
    ["NAPMEI"]="Employment Index"
    ["NAPMSDI"]="Supplier Deliveries Index"
    ["NAPMII"]="Inventories Index"
)

for ID in "${!SERIES[@]}"; do
    RESPONSE=$(curl -s "https://api.stlouisfed.org/fred/series/observations?series_id=$ID&api_key=$API_KEY&file_type=json&limit=1&sort_order=desc")

    if echo "$RESPONSE" | grep -q '"observations"'; then
        VALUE=$(echo "$RESPONSE" | python3 -c "import sys, json; d=json.load(sys.stdin); obs=d['observations'][0]; print(f\"{obs['date']}: {obs['value']}\")" 2>/dev/null)
        echo "✅ $ID (${SERIES[$ID]}): $VALUE"
    else
        echo "❌ $ID not found"
    fi
done

echo ""
echo "Recommendation: Use New Orders Index (NAPMNOI) as best PMI proxy"
