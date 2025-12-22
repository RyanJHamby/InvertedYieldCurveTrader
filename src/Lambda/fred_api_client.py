"""
FRED API Client - Federal Reserve Economic Data

Fetches economic indicators from the St. Louis Fed FRED API.
Documentation: https://fred.stlouisfed.org/docs/api/fred/

Author: Ryan Hamby
Created: 2025-12-17
"""

import requests
import json
from typing import Dict, List, Optional
from datetime import datetime, timedelta


class FREDAPIClient:
    """Client for fetching economic data from FRED API."""

    BASE_URL = "https://api.stlouisfed.org/fred/series/observations"

    # FRED Series IDs for our 7 economic indicators
    SERIES_IDS = {
        'inflation': 'CPIAUCSL',           # Consumer Price Index
        'treasury_10y': 'DGS10',           # 10-Year Treasury Yield
        'treasury_2y': 'DGS2',             # 2-Year Treasury Yield
        'fed_funds_rate': 'FEDFUNDS',      # Federal Funds Rate
        'unemployment': 'UNRATE',          # Unemployment Rate
        'gdp': 'A191RL1Q225SBEA',          # Real GDP Growth Rate
        'consumer_sentiment': 'UMCSENT',   # University of Michigan Consumer Sentiment
        'ism_manufacturing': 'NAPM'        # ISM Manufacturing PMI
    }

    def __init__(self, api_key: str):
        """
        Initialize FRED API client.

        Args:
            api_key: FRED API key from https://fred.stlouisfed.org/

        Raises:
            ValueError: If api_key is None or empty
        """
        if not api_key:
            raise ValueError("FRED API key cannot be None or empty")

        self.api_key = api_key
        self.session = requests.Session()  # Reuse HTTP connections

    def fetch_series(
        self,
        series_id: str,
        limit: int = 100,
        sort_order: str = 'desc',
        observation_start: Optional[str] = None,
        observation_end: Optional[str] = None
    ) -> Dict:
        """
        Fetch observations for a specific FRED series.

        Args:
            series_id: FRED series ID (e.g., 'CPIAUCSL')
            limit: Number of observations to fetch (default: 100)
            sort_order: 'asc' or 'desc' (default: 'desc' for most recent first)
            observation_start: Start date in YYYY-MM-DD format (optional)
            observation_end: End date in YYYY-MM-DD format (optional)

        Returns:
            Dict containing FRED API response with observations

        Raises:
            requests.exceptions.RequestException: If API request fails
            ValueError: If response is invalid or missing data
        """
        params = {
            'series_id': series_id,
            'api_key': self.api_key,
            'file_type': 'json',
            'limit': limit,
            'sort_order': sort_order
        }

        if observation_start:
            params['observation_start'] = observation_start
        if observation_end:
            params['observation_end'] = observation_end

        try:
            response = self.session.get(self.BASE_URL, params=params, timeout=10)
            response.raise_for_status()

            data = response.json()

            # Validate response
            if 'observations' not in data:
                raise ValueError(f"Invalid FRED API response: missing 'observations' key. Response: {data}")

            return data

        except requests.exceptions.Timeout:
            raise requests.exceptions.RequestException(f"FRED API request timed out for series {series_id}")
        except requests.exceptions.RequestException as e:
            raise requests.exceptions.RequestException(f"FRED API request failed for series {series_id}: {e}")

    def fetch_latest_value(self, series_id: str, num_values: int = 1) -> List[Dict]:
        """
        Fetch the most recent N values for a series.

        Args:
            series_id: FRED series ID
            num_values: Number of recent values to fetch (default: 1)

        Returns:
            List of dicts with 'date' and 'value' keys

        Example:
            >>> client.fetch_latest_value('CPIAUCSL', num_values=3)
            [
                {'date': '2025-11-01', 'value': 314.540},
                {'date': '2025-10-01', 'value': 314.211},
                {'date': '2025-09-01', 'value': 313.890}
            ]
        """
        data = self.fetch_series(series_id, limit=num_values, sort_order='desc')

        results = []
        for obs in data['observations']:
            # Filter out missing values (marked as '.')
            if obs['value'] != '.':
                results.append({
                    'date': obs['date'],
                    'value': float(obs['value'])
                })

        return results

    def fetch_inflation(self, num_values: int = 10) -> List[Dict]:
        """Fetch latest CPI (inflation) data."""
        return self.fetch_latest_value(self.SERIES_IDS['inflation'], num_values)

    def fetch_treasury_10y(self, num_values: int = 90) -> List[Dict]:
        """Fetch latest 10-Year Treasury yield data."""
        return self.fetch_latest_value(self.SERIES_IDS['treasury_10y'], num_values)

    def fetch_treasury_2y(self, num_values: int = 90) -> List[Dict]:
        """Fetch latest 2-Year Treasury yield data."""
        return self.fetch_latest_value(self.SERIES_IDS['treasury_2y'], num_values)

    def fetch_fed_funds_rate(self, num_values: int = 12) -> List[Dict]:
        """Fetch latest Federal Funds Rate data."""
        return self.fetch_latest_value(self.SERIES_IDS['fed_funds_rate'], num_values)

    def fetch_unemployment(self, num_values: int = 12) -> List[Dict]:
        """Fetch latest unemployment rate data."""
        return self.fetch_latest_value(self.SERIES_IDS['unemployment'], num_values)

    def fetch_gdp(self, num_values: int = 8) -> List[Dict]:
        """Fetch latest GDP growth rate data (quarterly)."""
        return self.fetch_latest_value(self.SERIES_IDS['gdp'], num_values)

    def fetch_consumer_sentiment(self, num_values: int = 12) -> List[Dict]:
        """Fetch latest University of Michigan Consumer Sentiment data."""
        return self.fetch_latest_value(self.SERIES_IDS['consumer_sentiment'], num_values)

    def fetch_ism_manufacturing(self, num_values: int = 12) -> List[Dict]:
        """Fetch latest ISM Manufacturing PMI data."""
        return self.fetch_latest_value(self.SERIES_IDS['ism_manufacturing'], num_values)

    def fetch_all_indicators(self) -> Dict[str, List[Dict]]:
        """
        Fetch all 8 economic indicators in one call.

        Returns:
            Dict mapping indicator names to their latest values

        Example:
            {
                'inflation': [{'date': '2025-11-01', 'value': 314.540}, ...],
                'treasury_10y': [{'date': '2025-12-16', 'value': 4.38}, ...],
                ...
            }
        """
        return {
            'inflation': self.fetch_inflation(),
            'treasury_10y': self.fetch_treasury_10y(),
            'treasury_2y': self.fetch_treasury_2y(),
            'fed_funds_rate': self.fetch_fed_funds_rate(),
            'unemployment': self.fetch_unemployment(),
            'gdp': self.fetch_gdp(),
            'consumer_sentiment': self.fetch_consumer_sentiment(),
            'ism_manufacturing': self.fetch_ism_manufacturing()
        }

    def calculate_inverted_yield_curve(self, num_days: int = 10) -> List[Dict]:
        """
        Calculate inverted yield curve spread (10Y - 2Y).

        A negative spread indicates an inverted yield curve, historically
        a predictor of recession.

        Args:
            num_days: Number of days to calculate

        Returns:
            List of dicts with 'date' and 'spread' keys

        Example:
            >>> client.calculate_inverted_yield_curve(num_days=3)
            [
                {'date': '2025-12-16', 'spread': 0.05},   # Normal
                {'date': '2025-12-15', 'spread': -0.10},  # Inverted!
                {'date': '2025-12-14', 'spread': -0.08}   # Inverted!
            ]
        """
        treasury_10y = self.fetch_treasury_10y(num_values=num_days)
        treasury_2y = self.fetch_treasury_2y(num_values=num_days)

        # Create date-indexed dicts for easy lookup
        yields_10y = {obs['date']: obs['value'] for obs in treasury_10y}
        yields_2y = {obs['date']: obs['value'] for obs in treasury_2y}

        # Calculate spreads for dates present in both series
        spreads = []
        for date in sorted(yields_10y.keys(), reverse=True):
            if date in yields_2y:
                spread = yields_10y[date] - yields_2y[date]
                spreads.append({'date': date, 'spread': spread})

                if len(spreads) >= num_days:
                    break

        return spreads

    def close(self):
        """Close the HTTP session."""
        self.session.close()

    def __enter__(self):
        """Context manager entry."""
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        """Context manager exit - close session."""
        self.close()


# Example usage
if __name__ == '__main__':
    import os

    # Get API key from environment variable
    api_key = os.getenv('FRED_API_KEY')
    if not api_key:
        print("Error: FRED_API_KEY environment variable not set")
        print("Set it with: export FRED_API_KEY='your_api_key_here'")
        exit(1)

    # Use context manager for automatic cleanup
    with FREDAPIClient(api_key) as client:
        # Fetch latest inflation
        print("Latest CPI (Inflation):")
        inflation = client.fetch_inflation(num_values=3)
        for obs in inflation:
            print(f"  {obs['date']}: {obs['value']}")

        print("\nInverted Yield Curve Spread (10Y - 2Y):")
        spreads = client.calculate_inverted_yield_curve(num_days=5)
        for obs in spreads:
            status = "INVERTED!" if obs['spread'] < 0 else "Normal"
            print(f"  {obs['date']}: {obs['spread']:.2f}% ({status})")

        print("\nAll indicators:")
        all_data = client.fetch_all_indicators()
        for indicator, values in all_data.items():
            if values:
                print(f"  {indicator}: {values[0]['value']} (as of {values[0]['date']})")
