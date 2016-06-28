// Written by Shawn Xiang

module.exports = [
  {
    "type": "heading",
    "defaultValue": "App Configuration"
  },
  {
    "type": "section",
    "items": [
      {
      "type": "slider",
      "messageKey": "WorkTime",
      "defaultValue": 25,
      "label": "Pomodoro Length",
      "description": "Recommended: 25 mins",
      "min": 5,
      "max": 60,
      "step": 1
      },
      {
      "type": "slider",
      "messageKey": "RestTime",
      "defaultValue": 5,
      "label": "Break Length",
      "description": "Recommended: 5 mins",
      "min": 1,
      "max": 30,
      "step": 1
      },
      {
      "type": "slider",
      "messageKey": "LongRestTime",
      "defaultValue": 15,
      "label": "Break Length",
      "description": "Recommended: 5 mins",
      "min": 1,
      "max": 30,
      "step": 1
      },
       {
      "type": "slider",
      "messageKey": "LongRestFrequency",
      "defaultValue": 4,
      "label": "Frequency of long break",
      "description": "Recommended: 4 breaks",
      "min": 1,
      "max": 12,
      "step": 1
      },
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];