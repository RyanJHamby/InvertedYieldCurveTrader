import * as cdk from 'aws-cdk-lib';
import * as events from 'aws-cdk-lib/aws-events';
import * as targets from 'aws-cdk-lib/aws-events-targets';
import * as lambda from 'aws-cdk-lib/aws-lambda';
import { Construct } from 'constructs';

interface EventBridgeStackProps extends cdk.StackProps {
  environment: string;
  lambdaFunction: lambda.Function;
}

export class EventBridgeStack extends cdk.Stack {
  public readonly rule: events.Rule;

  constructor(scope: Construct, id: string, props: EventBridgeStackProps) {
    super(scope, id, props);

    const ruleName = `InvertedYieldTrader${props.environment.charAt(0).toUpperCase() + props.environment.slice(1)}DailyTrigger`;

    // Create EventBridge rule for daily execution at 12 UTC
    this.rule = new events.Rule(this, 'DailyTriggerRule', {
      ruleName,
      schedule: events.Schedule.cron({
        minute: '0',
        hour: '12',
        month: '*',
        weekDay: '*',
        year: '*',
      }),
      description: 'Daily trigger for InvertedYieldCurveTrader analysis',
    });

    // Add Lambda as target
    this.rule.addTarget(new targets.LambdaFunction(props.lambdaFunction));

    // Output rule ARN
    new cdk.CfnOutput(this, 'RuleArn', {
      value: this.rule.ruleArn,
      exportName: `${id}-RuleArn`,
    });

    new cdk.CfnOutput(this, 'RuleName', {
      value: this.rule.ruleName,
      exportName: `${id}-RuleName`,
    });
  }
}
