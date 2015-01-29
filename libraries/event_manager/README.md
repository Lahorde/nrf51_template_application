### Description
Event manager for nRF51 target
Wrapper around SDK event manager (in SDK_PATH/your_target/app_common/app_scheduler.c)
Provides event broadcasting over some objects listening on events

### Prerequisities
nRF51 target 
nRF51 SDK to build app_scheduler
some delay methods in order to have a millis() implementation