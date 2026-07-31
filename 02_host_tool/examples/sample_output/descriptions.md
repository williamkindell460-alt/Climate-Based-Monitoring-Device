# Descriptions
## Anomaly definition
- Anomalies are defined as impossible jumps or values that do not occur naturally. The purpose of this output is to diagnose issues with sensors or data handling.
## Algorithm description
### The algorithms used are as simple as possible while maintaining speed and reliability
- The hard anomaly detection compares each value in the CSV output to known impossible values for the given environment and sensors, and this is used to diagnose sensor issues or data handling issues.
- The soft anomaly detection does the same thing; however, it tests possible, but highly unlikely values in a normal environment, and this is used to indicate possible failures of the configuration of the sensors or the sensors themselves.
- The trend anomaly detection goes through the entire list comparing the second value to the first, taking their difference, and comparing that to known impossible fluctuations in a normal environment.