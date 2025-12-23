# Troubleshooting FRED API "missing 'observations' key" Error

## Quick Diagnosis

Run the debug program to see the actual API response:

```bash
./debug_fred
```

This will show you the raw JSON response from FRED, which will help identify the issue.

## Common Causes & Solutions

### 1. Invalid or Unactivated API Key

**Symptom:** FRED returns an error message instead of data

**FRED Error Response Example:**
```json
{
  "error_code": 400,
  "error_message": "Bad Request. The value for variable api_key is not registered."
}
```

**Solution:**
1. Double-check your API key is correct
2. Make sure you copied the entire key (usually 32 characters)
3. Verify the API key at: https://fred.stlouisfed.org/docs/api/api_key.html
4. Try requesting a new API key if needed

**Test your API key in browser:**
```
https://api.stlouisfed.org/fred/series/observations?series_id=CPIAUCSL&api_key=YOUR_KEY_HERE&file_type=json&limit=1
```

You should see JSON with `"observations": [...]` in the response.

---

### 2. API Key Not Set in Environment

**Symptom:** Program fails before making any API calls

**Solution:**
```bash
# Check if the environment variable is set
echo $FRED_API_KEY

# If empty, set it:
export FRED_API_KEY="your_actual_key_here"

# Verify it's set:
echo $FRED_API_KEY

# Run test again:
./test_fred
```

---

### 3. Network/Firewall Issues

**Symptom:** Request times out or returns HTML error page

**Solution:**
```bash
# Test connection to FRED API
curl "https://api.stlouisfed.org/fred/series/observations?series_id=CPIAUCSL&api_key=YOUR_KEY&file_type=json&limit=1"

# If this fails, check:
# - Your internet connection
# - Firewall settings
# - VPN/proxy configuration
```

---

### 4. Rate Limiting (Very Rare)

**Symptom:** FRED returns rate limit error

**FRED Rate Limit Response:**
```json
{
  "error_code": 429,
  "error_message": "Too Many Requests"
}
```

**Solution:**
- Wait a few minutes and try again
- FRED has very generous limits, so this is unlikely
- If persists, contact FRED support

---

## Step-by-Step Debugging

### Step 1: Run Debug Program

```bash
./debug_fred
```

Look at the "Raw JSON Response" output. It should look like:
```json
{
  "realtime_start": "2025-12-22",
  "realtime_end": "2025-12-22",
  "observation_start": "1776-07-04",
  "observation_end": "9999-12-31",
  "units": "lin",
  "output_type": 1,
  "file_type": "json",
  "order_by": "observation_date",
  "sort_order": "desc",
  "count": 3,
  "offset": 0,
  "limit": 3,
  "observations": [
    {
      "realtime_start": "2025-12-22",
      "realtime_end": "2025-12-22",
      "date": "2025-11-01",
      "value": "314.540"
    },
    ...
  ]
}
```

**If you see `"error_code"` or `"error_message"` instead**, your API key is invalid.

### Step 2: Verify API Key Format

FRED API keys are typically 32 alphanumeric characters:
```
Example: a1b2c3d4e5f6g7h8i9j0k1l2m3n4o5p6
```

Check your key:
```bash
echo $FRED_API_KEY | wc -c
# Should print 33 (32 chars + newline)
```

### Step 3: Test Directly with cURL

```bash
# Replace YOUR_KEY with your actual API key
curl "https://api.stlouisfed.org/fred/series/observations?series_id=CPIAUCSL&api_key=YOUR_KEY&file_type=json&limit=1" | python3 -m json.tool
```

This should return nicely formatted JSON with an `"observations"` array.

### Step 4: Check for Typos in Environment Variable

```bash
# Make sure there are no extra spaces or quotes
export FRED_API_KEY="abc123..."  # ✅ Correct

export FRED_API_KEY=" abc123..." # ❌ Leading space
export FRED_API_KEY="abc123... " # ❌ Trailing space
export FRED_API_KEY='"abc123..."' # ❌ Extra quotes
```

---

## Still Having Issues?

### Get a Fresh API Key

1. Go to https://fred.stlouisfed.org/docs/api/api_key.html
2. Click "Delete API Key" to remove old key
3. Click "Request API Key" to get a new one
4. Update your environment variable with the new key

### Test with Known Working Key

If you suspect your key is bad, you can temporarily test with the FRED demo key:

**WARNING:** Demo keys have very limited quotas. Only use for quick testing!

```bash
# Test with a simple curl to verify FRED API is working
curl "https://api.stlouisfed.org/fred/series?series_id=GNPCA&api_key=abcdefghijklmnopqrstuvwxyz123456&file_type=json"
```

---

## Expected Behavior After Fix

Once your API key is working correctly, you should see:

```
Initializing FRED API Client...

=== Test 1: Fetching CPI (Inflation) Data ===

Latest CPI Values:
  2025-11-01: 314.540
  2025-10-01: 314.211
  2025-09-01: 313.890
  2025-08-01: 313.567
  2025-07-01: 313.245

=== Test 2: Fetching Treasury Yields ===
...

=== All Tests Passed! ===
FRED API Client is working correctly.
```

---

## Contact Support

If none of these solutions work:

1. **FRED Support:** https://fred.stlouisfed.org/contactus/
2. **Check FRED Status:** https://fred.stlouisfed.org/ (look for service announcements)
3. **API Documentation:** https://fred.stlouisfed.org/docs/api/fred/

---

## Quick Reference Commands

```bash
# Check API key is set
echo $FRED_API_KEY

# Run debug program
./debug_fred

# Test with curl
curl "https://api.stlouisfed.org/fred/series/observations?series_id=CPIAUCSL&api_key=$FRED_API_KEY&file_type=json&limit=1"

# Recompile if you made code changes
g++ -std=c++20 -I./src -I/opt/homebrew/opt/nlohmann-json/include \
    src/DataProviders/FREDDataClient.cpp test_fred_client.cpp -lcurl -o test_fred

# Run full test
./test_fred
```
