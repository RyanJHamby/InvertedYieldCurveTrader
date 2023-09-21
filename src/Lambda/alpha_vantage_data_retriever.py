import requests
import boto3
from datetime import date

class AlphaVantageDataRetriever:
    def __init__(self, api_key, max_results):
        self.api_key = api_key
        self.max_results = max_results
        
    def fetch_data_and_upload_to_s3(self, api_template_url, bucket_name, object_key):
        api_url = f"https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol={symbol}&interval=1min&apikey={self.api_key}"
        results = self.retrieve(api_url)
        self.upload_to_s3(bucket_name, object_key, results)

    def retrieve(self, api_url):
        try:
            print("trying api")
            response = requests.get(api_url)
            print("got response")
            response.raise_for_status()  # Raise an exception for HTTP errors

            return response.text
        except requests.exceptions.RequestException as e:
            return f"Request failed: {e}"
            
    def upload_to_s3(self, bucket_name, object_key, data):
        try:
            s3 = boto3.client('s3')

            if isinstance(data, str):
                s3.put_object(Bucket=bucket_name, Key=object_key, Body=data)
            elif isinstance(data, bytes):
                s3.put_object(Bucket=bucket_name, Key=object_key, Body=data)
            else:
                raise ValueError("Data must be a string or bytes")

            print(f"Data successfully uploaded to S3: s3://{bucket_name}/{object_key}")
            return True
        except Exception as e:
            print(f"Failed to upload data to S3: {e}")
            return False
        
def fetch(event, context):
    bucket_name = 'alpha-insights'
    object_key = 'time-series/' + str(date.today())
    
    data_retriever = AlphaVantageDataRetriever(api_key, max_results)
    stock_data = data_retriever.fetch_data_and_upload_to_s3(symbol, bucket_name, object_key)

    print(stock_data)
    
stock_of_interest = "VOO"
api_key = "D3G5VY7GCOEB887N"

data_extraction_categories = {
    "time_series": {
        "s3_object_key": "time-series",
        "api_template_url": f"https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol={stock_of_interest}&interval=5min&outputsize=full&apikey={api_key}"
    },
    "yield" : {
        "s3_object_key": "yield",
        "api_template_url": f"https://www.alphavantage.co/query?function=TREASURY_YIELD&interval=daily&maturity=10year&apikey={api_key}"
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
