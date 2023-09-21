stock_of_interest = "VOO"
api_key = "D3G5VY7GCOEB887N"

alpha_vantage_constants = {
    "time_series": {
        "s3_object_key": "time-series",
        "api_template_url": f"https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol={stock_of_interest}&interval=1min&outputsize=full&apikey={api_key}"
    },
    "yield-10-year" : {
        "s3_object_key": "yield-10-year",
        "api_template_url": f"https://www.alphavantage.co/query?function=TREASURY_YIELD&interval=daily&maturity=10year&apikey={api_key}"
    },
    "yield-2-year" : {
        "s3_object_key": "yield-2-year",
        "api_template_url": f"https://www.alphavantage.co/query?function=TREASURY_YIELD&interval=daily&maturity=2year&apikey={api_key}"
    },
    "inflation" : {
        "s3_object_key": "inflation",
        "api_template_url": f"https://www.alphavantage.co/query?function=INFLATION&apikey=demo&apikey={api_key}"
    },
    "gdp" : {
        "s3_object_key": "gdp",
        "api_template_url": f"https://www.alphavantage.co/query?function=REAL_GDP&interval=quarterly&apikey={api_key}"
    },
    "interest_rate" : {
        "s3_object_key": "interest-rate",
        "api_template_url": f"https://www.alphavantage.co/query?function=FEDERAL_FUNDS_RATE&interval=daily&apikey={api_key}"
    },
    "s&p" : {
        "s3_object_key": "s&p",
        "api_template_url": f"https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol={stock_of_interest}&apikey={api_key}"
    }
}
