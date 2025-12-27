import * as cdk from 'aws-cdk-lib';
import * as lambda from 'aws-cdk-lib/aws-lambda';
import * as iam from 'aws-cdk-lib/aws-iam';
import * as path from 'path';
import { Construct } from 'constructs';

interface LambdaStackProps extends cdk.StackProps {
  environment: string;
  s3BucketName: string;
  lambdaRole: iam.Role;
  fredApiKey: string;
  alphaVantageApiKey: string;
}

export class LambdaStack extends cdk.Stack {
  public readonly lambdaFunction: lambda.Function;

  constructor(scope: Construct, id: string, props: LambdaStackProps) {
    super(scope, id, props);

    const functionName = `InvertedYieldTrader${props.environment.charAt(0).toUpperCase() + props.environment.slice(1)}`;

    // Get path to lambda package
    const lambdaPackagePath = path.join(__dirname, '../../..', 'lambda-function.zip');

    // Create Lambda function
    this.lambdaFunction = new lambda.Function(this, 'TraderFunction', {
      functionName,
      runtime: lambda.Runtime.PROVIDED_AL2,
      handler: 'bootstrap',
      code: lambda.Code.fromAsset(lambdaPackagePath),
      role: props.lambdaRole,
      timeout: cdk.Duration.seconds(600),
      memorySize: 512,
      environment: {
        FRED_API_KEY: props.fredApiKey,
        ALPHA_VANTAGE_API_KEY: props.alphaVantageApiKey,
        S3_BUCKET: props.s3BucketName,
      },
      description: 'InvertedYieldCurveTrader daily analysis function',
    });

    // Output function ARN
    new cdk.CfnOutput(this, 'FunctionArn', {
      value: this.lambdaFunction.functionArn,
      exportName: `${id}-Arn`,
    });

    new cdk.CfnOutput(this, 'FunctionName', {
      value: this.lambdaFunction.functionName,
      exportName: `${id}-Name`,
    });
  }
}
