import * as cdk from 'aws-cdk-lib';
import * as iam from 'aws-cdk-lib/aws-iam';
import { Construct } from 'constructs';

interface IamStackProps extends cdk.StackProps {
  environment: string;
  s3BucketName: string;
}

export class IamStack extends cdk.Stack {
  public readonly lambdaRole: iam.Role;

  constructor(scope: Construct, id: string, props: IamStackProps) {
    super(scope, id, props);

    const roleName = `inverted-yield-trader-${props.environment}-lambda-role`;

    // Create Lambda execution role
    this.lambdaRole = new iam.Role(this, 'LambdaExecutionRole', {
      roleName,
      assumedBy: new iam.ServicePrincipal('lambda.amazonaws.com'),
      description: 'Execution role for InvertedYieldCurveTrader Lambda',
    });

    // Add basic Lambda execution policy (CloudWatch Logs)
    this.lambdaRole.addManagedPolicy(
      iam.ManagedPolicy.fromAwsManagedPolicyName('service-role/AWSLambdaBasicExecutionRole')
    );

    // Add S3 permissions
    this.lambdaRole.addToPolicy(
      new iam.PolicyStatement({
        effect: iam.Effect.ALLOW,
        actions: ['s3:PutObject', 's3:GetObject'],
        resources: [`arn:aws:s3:::${props.s3BucketName}/*`],
      })
    );

    // Output role ARN
    new cdk.CfnOutput(this, 'LambdaRoleArn', {
      value: this.lambdaRole.roleArn,
      exportName: `${id}-RoleArn`,
    });
  }
}
