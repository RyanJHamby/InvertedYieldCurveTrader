import requests
import boto3
import json
from datetime import date

class AlphaVantageDataRetriever:
    def __init__(self):
        self.api_key = "D3G5VY7GCOEB887N"
        
    def fetch_data_and_upload_to_s3(self, api_template_url, bucket_name, object_key):
        api_url = api_template_url.format(self.api_key)
        results = self.retrieve(api_url)
        
        if object_key.startswith("time-series"):
            results = filter_json_by_recent_date(results)
        self.upload_to_s3(bucket_name, object_key, results)

    def retrieve(self, api_url):
        try:
            response = requests.get(api_url)
            response.raise_for_status()  # Raise an exception for HTTP errors

            return response.text
        except requests.exceptions.RequestException as e:
            return f"Request failed: {e}"
            
    def upload_to_s3(self, bucket_name, object_key, data):
        try:
            s3 = boto3.client('s3')

            if isinstance(data, str) or isinstance(data, bytes):
                s3.put_object(Bucket=bucket_name, Key=object_key, Body=data)
            else:
                raise ValueError("Data must be a string or bytes")

            print(f"Data successfully uploaded to S3: s3://{bucket_name}/{object_key}")
            return True
        except Exception as e:
            print(f"Failed to upload data to S3: {e}")
            return False
            
def filter_json_by_recent_date(json_data):
    data = json.loads(json_data)
    
    recent_date = data["Meta Data"]["3. Last Refreshed"]
    
    filtered_time_series = {
        k: v for k, v in data["Time Series (1min)"].items() if k.startswith(recent_date.split()[0])
    }
    
    filtered_data = {
        "Meta Data": data["Meta Data"],
        "Time Series (1min)": filtered_time_series
    }
    
    filtered_json = json.dumps(filtered_data, indent=4)
    
    return filtered_json
        
def lambda_handler(event, context):
    bucket_name = 'alpha-insights'
    data_retriever = AlphaVantageDataRetriever()
    
    json_file_path = "alpha_vantage_constants.json"

    with open(json_file_path, "r") as json_file:
        alpha_vantage_constants = json.load(json_file)
        print(alpha_vantage_constants)
    
    for k, v in alpha_vantage_constants.items():
        object_key = v["s3_object_key"] + "/" + str(date.today())
        stock_data = data_retriever.fetch_data_and_upload_to_s3(v["api_template_url"], bucket_name, object_key)

# run python alpha_vantage_data_retriever.py to run this locally. TODO: set up EventBridge to run nightly
lambda_handler(1, 2)
