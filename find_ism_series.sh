#!/bin/bash
# Script to test various ISM Manufacturing series IDs on FRED

API_KEY="${FRED_API_KEY}"

if [ -z "$API_KEY" ]; then
    echo "Error: FRED_API_KEY not set"
    exit 1
fi

echo "Testing possible ISM Manufacturing series IDs..."
echo "================================================"

# List of possible series IDs to test
SERIES_IDS=(
    "NAPMPMI"
    "MANEMP"
    "NAPM"
    "NAPMNOI"
    "NAPMPI"
    "NAPMSDI"
    "NAPMEI"
    "ISM/MAN"
    "ISMMFG"
    "NAPMII"
    "NAPMNI"
)

for SERIES in "${SERIES_IDS[@]}"; do
    echo ""
    echo "Testing: $SERIES"
    RESPONSE=$(curl -s "https://api.stlouisfed.org/fred/series/observations?series_id=$SERIES&api_key=$API_KEY&file_type=json&limit=1")

    if echo "$RESPONSE" | grep -q "error_code"; then
        echo "  ❌ Does not exist"
    else
        echo "  ✅ EXISTS!"
        echo "  Latest value:"
        echo "$RESPONSE" | python3 -c "import sys, json; data=json.load(sys.stdin); obs=data.get('observations',[]); print(f\"    {obs[0]['date']}: {obs[0]['value']}\" if obs and obs[0]['value'] != '.' else '    No data available')" 2>/dev/null
    fi
done

echo ""
echo "================================================"
echo "Done! Use the series ID marked with ✅"
