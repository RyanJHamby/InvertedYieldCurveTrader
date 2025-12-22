"""
Unit tests for FRED API Client

Run with: pytest tests/test_fred_api_client.py -v
Or: python -m pytest tests/test_fred_api_client.py -v --cov=src/Lambda

Author: Ryan Hamby
Created: 2025-12-17
"""

import pytest
import requests
from unittest.mock import Mock, patch, MagicMock
import sys
import os

# Add src to path for imports
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../src/Lambda')))

from fred_api_client import FREDAPIClient


class TestFREDAPIClientInitialization:
    """Test client initialization and validation."""

    def test_init_with_valid_api_key(self):
        """Should initialize successfully with valid API key."""
        client = FREDAPIClient("test_api_key_123")
        assert client.api_key == "test_api_key_123"
        assert client.session is not None

    def test_init_with_none_api_key(self):
        """Should raise ValueError when API key is None."""
        with pytest.raises(ValueError, match="API key cannot be None or empty"):
            FREDAPIClient(None)

    def test_init_with_empty_api_key(self):
        """Should raise ValueError when API key is empty string."""
        with pytest.raises(ValueError, match="API key cannot be None or empty"):
            FREDAPIClient("")


class TestFREDAPIClientFetchSeries:
    """Test fetching series data from FRED API."""

    @patch('requests.Session.get')
    def test_fetch_series_success(self, mock_get):
        """Should successfully fetch series data."""
        # Mock successful API response
        mock_response = Mock()
        mock_response.status_code = 200
        mock_response.json.return_value = {
            'realtime_start': '2025-12-17',
            'realtime_end': '2025-12-17',
            'observation_start': '1776-07-04',
            'observation_end': '9999-12-31',
            'units': 'lin',
            'output_type': 1,
            'file_type': 'json',
            'order_by': 'observation_date',
            'sort_order': 'desc',
            'count': 3,
            'offset': 0,
            'limit': 3,
            'observations': [
                {'realtime_start': '2025-12-17', 'realtime_end': '2025-12-17',
                 'date': '2025-11-01', 'value': '314.540'},
                {'realtime_start': '2025-12-17', 'realtime_end': '2025-12-17',
                 'date': '2025-10-01', 'value': '314.211'},
                {'realtime_start': '2025-12-17', 'realtime_end': '2025-12-17',
                 'date': '2025-09-01', 'value': '313.890'}
            ]
        }
        mock_get.return_value = mock_response

        client = FREDAPIClient("test_key")
        result = client.fetch_series('CPIAUCSL', limit=3)

        assert 'observations' in result
        assert len(result['observations']) == 3
        assert result['observations'][0]['value'] == '314.540'

        # Verify correct API call
        mock_get.assert_called_once()
        call_args = mock_get.call_args
        assert call_args[0][0] == FREDAPIClient.BASE_URL
        assert call_args[1]['params']['series_id'] == 'CPIAUCSL'
        assert call_args[1]['params']['limit'] == 3

    @patch('requests.Session.get')
    def test_fetch_series_with_date_range(self, mock_get):
        """Should pass date range parameters correctly."""
        mock_response = Mock()
        mock_response.status_code = 200
        mock_response.json.return_value = {'observations': []}
        mock_get.return_value = mock_response

        client = FREDAPIClient("test_key")
        client.fetch_series(
            'CPIAUCSL',
            observation_start='2025-01-01',
            observation_end='2025-12-31'
        )

        call_args = mock_get.call_args
        assert call_args[1]['params']['observation_start'] == '2025-01-01'
        assert call_args[1]['params']['observation_end'] == '2025-12-31'

    @patch('requests.Session.get')
    def test_fetch_series_missing_observations(self, mock_get):
        """Should raise ValueError when response is missing observations."""
        mock_response = Mock()
        mock_response.status_code = 200
        mock_response.json.return_value = {
            'error': 'Bad Request',
            'error_message': 'Invalid series ID'
        }
        mock_get.return_value = mock_response

        client = FREDAPIClient("test_key")
        with pytest.raises(ValueError, match="missing 'observations' key"):
            client.fetch_series('INVALID_SERIES')

    @patch('requests.Session.get')
    def test_fetch_series_timeout(self, mock_get):
        """Should raise RequestException on timeout."""
        mock_get.side_effect = requests.exceptions.Timeout()

        client = FREDAPIClient("test_key")
        with pytest.raises(requests.exceptions.RequestException, match="timed out"):
            client.fetch_series('CPIAUCSL')

    @patch('requests.Session.get')
    def test_fetch_series_http_error(self, mock_get):
        """Should raise RequestException on HTTP error."""
        mock_response = Mock()
        mock_response.status_code = 500
        mock_response.raise_for_status.side_effect = requests.exceptions.HTTPError("500 Server Error")
        mock_get.return_value = mock_response

        client = FREDAPIClient("test_key")
        with pytest.raises(requests.exceptions.RequestException, match="failed"):
            client.fetch_series('CPIAUCSL')


class TestFREDAPIClientFetchLatestValue:
    """Test fetching latest values for series."""

    @patch('requests.Session.get')
    def test_fetch_latest_value_single(self, mock_get):
        """Should fetch single latest value."""
        mock_response = Mock()
        mock_response.status_code = 200
        mock_response.json.return_value = {
            'observations': [
                {'date': '2025-11-01', 'value': '314.540'}
            ]
        }
        mock_get.return_value = mock_response

        client = FREDAPIClient("test_key")
        result = client.fetch_latest_value('CPIAUCSL', num_values=1)

        assert len(result) == 1
        assert result[0]['date'] == '2025-11-01'
        assert result[0]['value'] == 314.540
        assert isinstance(result[0]['value'], float)

    @patch('requests.Session.get')
    def test_fetch_latest_value_multiple(self, mock_get):
        """Should fetch multiple latest values."""
        mock_response = Mock()
        mock_response.status_code = 200
        mock_response.json.return_value = {
            'observations': [
                {'date': '2025-11-01', 'value': '314.540'},
                {'date': '2025-10-01', 'value': '314.211'},
                {'date': '2025-09-01', 'value': '313.890'}
            ]
        }
        mock_get.return_value = mock_response

        client = FREDAPIClient("test_key")
        result = client.fetch_latest_value('CPIAUCSL', num_values=3)

        assert len(result) == 3
        assert result[0]['value'] == 314.540
        assert result[2]['value'] == 313.890

    @patch('requests.Session.get')
    def test_fetch_latest_value_filters_missing(self, mock_get):
        """Should filter out missing values (marked as '.')."""
        mock_response = Mock()
        mock_response.status_code = 200
        mock_response.json.return_value = {
            'observations': [
                {'date': '2025-12-16', 'value': '4.38'},
                {'date': '2025-12-15', 'value': '.'},  # Missing data
                {'date': '2025-12-14', 'value': '4.35'}
            ]
        }
        mock_get.return_value = mock_response

        client = FREDAPIClient("test_key")
        result = client.fetch_latest_value('DGS10', num_values=3)

        # Should only return 2 values (missing one filtered out)
        assert len(result) == 2
        assert result[0]['value'] == 4.38
        assert result[1]['value'] == 4.35


class TestFREDAPIClientIndicatorFetchers:
    """Test convenience methods for fetching specific indicators."""

    @patch.object(FREDAPIClient, 'fetch_latest_value')
    def test_fetch_inflation(self, mock_fetch):
        """Should fetch inflation data with correct series ID."""
        mock_fetch.return_value = [{'date': '2025-11-01', 'value': 314.540}]

        client = FREDAPIClient("test_key")
        result = client.fetch_inflation(num_values=10)

        mock_fetch.assert_called_once_with('CPIAUCSL', 10)
        assert result[0]['value'] == 314.540

    @patch.object(FREDAPIClient, 'fetch_latest_value')
    def test_fetch_treasury_10y(self, mock_fetch):
        """Should fetch 10-year treasury data."""
        mock_fetch.return_value = [{'date': '2025-12-16', 'value': 4.38}]

        client = FREDAPIClient("test_key")
        result = client.fetch_treasury_10y(num_values=90)

        mock_fetch.assert_called_once_with('DGS10', 90)

    @patch.object(FREDAPIClient, 'fetch_latest_value')
    def test_fetch_fed_funds_rate(self, mock_fetch):
        """Should fetch federal funds rate data."""
        mock_fetch.return_value = [{'date': '2025-11-01', 'value': 4.58}]

        client = FREDAPIClient("test_key")
        result = client.fetch_fed_funds_rate(num_values=12)

        mock_fetch.assert_called_once_with('FEDFUNDS', 12)

    @patch.object(FREDAPIClient, 'fetch_latest_value')
    def test_fetch_unemployment(self, mock_fetch):
        """Should fetch unemployment data."""
        mock_fetch.return_value = [{'date': '2025-11-01', 'value': 3.9}]

        client = FREDAPIClient("test_key")
        result = client.fetch_unemployment(num_values=12)

        mock_fetch.assert_called_once_with('UNRATE', 12)

    @patch.object(FREDAPIClient, 'fetch_latest_value')
    def test_fetch_gdp(self, mock_fetch):
        """Should fetch GDP data."""
        mock_fetch.return_value = [{'date': '2025-10-01', 'value': 2.8}]

        client = FREDAPIClient("test_key")
        result = client.fetch_gdp(num_values=8)

        mock_fetch.assert_called_once_with('A191RL1Q225SBEA', 8)

    @patch.object(FREDAPIClient, 'fetch_latest_value')
    def test_fetch_consumer_sentiment(self, mock_fetch):
        """Should fetch consumer sentiment data."""
        mock_fetch.return_value = [{'date': '2025-12-01', 'value': 74.0}]

        client = FREDAPIClient("test_key")
        result = client.fetch_consumer_sentiment(num_values=12)

        mock_fetch.assert_called_once_with('UMCSENT', 12)

    @patch.object(FREDAPIClient, 'fetch_latest_value')
    def test_fetch_ism_manufacturing(self, mock_fetch):
        """Should fetch ISM manufacturing data."""
        mock_fetch.return_value = [{'date': '2025-12-01', 'value': 48.4}]

        client = FREDAPIClient("test_key")
        result = client.fetch_ism_manufacturing(num_values=12)

        mock_fetch.assert_called_once_with('NAPM', 12)


class TestFREDAPIClientFetchAllIndicators:
    """Test fetching all indicators at once."""

    @patch.object(FREDAPIClient, 'fetch_inflation')
    @patch.object(FREDAPIClient, 'fetch_treasury_10y')
    @patch.object(FREDAPIClient, 'fetch_treasury_2y')
    @patch.object(FREDAPIClient, 'fetch_fed_funds_rate')
    @patch.object(FREDAPIClient, 'fetch_unemployment')
    @patch.object(FREDAPIClient, 'fetch_gdp')
    @patch.object(FREDAPIClient, 'fetch_consumer_sentiment')
    @patch.object(FREDAPIClient, 'fetch_ism_manufacturing')
    def test_fetch_all_indicators(self, mock_ism, mock_sentiment, mock_gdp,
                                   mock_unemployment, mock_fed, mock_2y, mock_10y, mock_inflation):
        """Should fetch all 8 indicators."""
        # Mock each indicator
        mock_inflation.return_value = [{'date': '2025-11-01', 'value': 314.540}]
        mock_10y.return_value = [{'date': '2025-12-16', 'value': 4.38}]
        mock_2y.return_value = [{'date': '2025-12-16', 'value': 4.33}]
        mock_fed.return_value = [{'date': '2025-11-01', 'value': 4.58}]
        mock_unemployment.return_value = [{'date': '2025-11-01', 'value': 3.9}]
        mock_gdp.return_value = [{'date': '2025-10-01', 'value': 2.8}]
        mock_sentiment.return_value = [{'date': '2025-12-01', 'value': 74.0}]
        mock_ism.return_value = [{'date': '2025-12-01', 'value': 48.4}]

        client = FREDAPIClient("test_key")
        result = client.fetch_all_indicators()

        # Verify all indicators present
        assert 'inflation' in result
        assert 'treasury_10y' in result
        assert 'treasury_2y' in result
        assert 'fed_funds_rate' in result
        assert 'unemployment' in result
        assert 'gdp' in result
        assert 'consumer_sentiment' in result
        assert 'ism_manufacturing' in result

        # Verify each method was called once
        mock_inflation.assert_called_once()
        mock_10y.assert_called_once()
        mock_2y.assert_called_once()
        mock_fed.assert_called_once()
        mock_unemployment.assert_called_once()
        mock_gdp.assert_called_once()
        mock_sentiment.assert_called_once()
        mock_ism.assert_called_once()


class TestFREDAPIClientYieldCurve:
    """Test yield curve calculation."""

    @patch.object(FREDAPIClient, 'fetch_treasury_10y')
    @patch.object(FREDAPIClient, 'fetch_treasury_2y')
    def test_calculate_inverted_yield_curve(self, mock_2y, mock_10y):
        """Should calculate yield curve spread (10Y - 2Y)."""
        # Mock treasury data
        mock_10y.return_value = [
            {'date': '2025-12-16', 'value': 4.38},
            {'date': '2025-12-15', 'value': 4.35},
            {'date': '2025-12-14', 'value': 4.32}
        ]
        mock_2y.return_value = [
            {'date': '2025-12-16', 'value': 4.33},
            {'date': '2025-12-15', 'value': 4.30},
            {'date': '2025-12-14', 'value': 4.28}
        ]

        client = FREDAPIClient("test_key")
        result = client.calculate_inverted_yield_curve(num_days=3)

        assert len(result) == 3
        assert result[0]['date'] == '2025-12-16'
        assert result[0]['spread'] == pytest.approx(0.05, abs=0.01)  # 4.38 - 4.33
        assert result[1]['date'] == '2025-12-15'
        assert result[1]['spread'] == pytest.approx(0.05, abs=0.01)  # 4.35 - 4.30

    @patch.object(FREDAPIClient, 'fetch_treasury_10y')
    @patch.object(FREDAPIClient, 'fetch_treasury_2y')
    def test_calculate_inverted_yield_curve_negative(self, mock_2y, mock_10y):
        """Should detect inverted yield curve (negative spread)."""
        # Mock inverted yield curve
        mock_10y.return_value = [
            {'date': '2025-12-16', 'value': 4.20},  # 10Y lower than 2Y!
        ]
        mock_2y.return_value = [
            {'date': '2025-12-16', 'value': 4.30},
        ]

        client = FREDAPIClient("test_key")
        result = client.calculate_inverted_yield_curve(num_days=1)

        assert len(result) == 1
        assert result[0]['spread'] == pytest.approx(-0.10, abs=0.01)  # Negative = inverted
        assert result[0]['spread'] < 0  # Inverted!

    @patch.object(FREDAPIClient, 'fetch_treasury_10y')
    @patch.object(FREDAPIClient, 'fetch_treasury_2y')
    def test_calculate_inverted_yield_curve_missing_dates(self, mock_2y, mock_10y):
        """Should handle missing dates in one series."""
        # 10Y has more data points than 2Y
        mock_10y.return_value = [
            {'date': '2025-12-16', 'value': 4.38},
            {'date': '2025-12-15', 'value': 4.35},
            {'date': '2025-12-14', 'value': 4.32}
        ]
        mock_2y.return_value = [
            {'date': '2025-12-16', 'value': 4.33},
            # Missing 2025-12-15
            {'date': '2025-12-14', 'value': 4.28}
        ]

        client = FREDAPIClient("test_key")
        result = client.calculate_inverted_yield_curve(num_days=3)

        # Should only return spreads for matching dates
        assert len(result) == 2
        dates = [r['date'] for r in result]
        assert '2025-12-16' in dates
        assert '2025-12-14' in dates
        assert '2025-12-15' not in dates  # Missing in 2Y data


class TestFREDAPIClientContextManager:
    """Test context manager functionality."""

    @patch('requests.Session.close')
    def test_context_manager(self, mock_close):
        """Should close session when exiting context."""
        with FREDAPIClient("test_key") as client:
            assert client.api_key == "test_key"

        # Session should be closed after exiting context
        mock_close.assert_called_once()

    def test_close_method(self):
        """Should have close method."""
        client = FREDAPIClient("test_key")
        client.close()  # Should not raise exception


class TestFREDAPIClientIntegration:
    """Integration tests (requires actual API key)."""

    @pytest.mark.integration
    @pytest.mark.skipif(not os.getenv('FRED_API_KEY'), reason="FRED_API_KEY not set")
    def test_real_api_call_inflation(self):
        """Integration test: Fetch real inflation data."""
        api_key = os.getenv('FRED_API_KEY')
        client = FREDAPIClient(api_key)

        result = client.fetch_inflation(num_values=1)

        assert len(result) >= 1
        assert 'date' in result[0]
        assert 'value' in result[0]
        assert result[0]['value'] > 100  # CPI should be > 100
        client.close()

    @pytest.mark.integration
    @pytest.mark.skipif(not os.getenv('FRED_API_KEY'), reason="FRED_API_KEY not set")
    def test_real_api_call_yield_curve(self):
        """Integration test: Calculate real yield curve."""
        api_key = os.getenv('FRED_API_KEY')
        client = FREDAPIClient(api_key)

        result = client.calculate_inverted_yield_curve(num_days=1)

        assert len(result) >= 1
        assert 'date' in result[0]
        assert 'spread' in result[0]
        # Spread should be reasonable (between -2% and +3%)
        assert -2.0 < result[0]['spread'] < 3.0
        client.close()


# Run tests
if __name__ == '__main__':
    pytest.main([__file__, '-v', '--cov=src/Lambda', '--cov-report=term-missing'])
