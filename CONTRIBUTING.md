# Contributing to Sonoff-Tasmota

**Any contribution helps our team and makes Tasmota better for the entire community!**

Everybody is welcome and invited to contribute to Sonoff-Tasmota Project by:

* Testing newly released features and reporting issues.
* Providing Pull Requests (Features, Proof of Concepts, Language files or Fixes)
* Contributing missing documentation for features and devices on our [Wiki](https://github.com/arendst/Sonoff-Tasmota/wiki/Contributing)

This document describes rules that are in effect for this repository, meant for handling issues by contributors in the issue tracker and PRs.

## Opening New Issues

**Issue tracker is NOT a general discussion forum!**
1. Opening an issue means that a problem exists in the code and should be addressed by the project contributors.
2. When opening an issue, it is required to fill out the presented template. The requested information is important! If the template is ignored or insufficient info about the issue is provided, the issue may be closed.
3. Questions of type "How do I..." or "Can you please help me with..." or "Can Tasmota do..." WILL NOT be handled here. Such questions should be directed at a discussion forum or to the Tasmota Support Chat. All issues of this type will be closed with a simple reference to this contributing policy.
4. Issues about topics already handled in the documentation will be closed in a similar manner.
5. Issues for unmerged PRs will be closed. If there is an issue with a PR, the explanation should be added to the PR itself.
6. Issues with accompanied investigation that shows the root of the problem should be given priority.
7. Duplicate issues will be closed.

## Triaging of Issues/PR's

1.	Any contributor to the project can participate in the triaging process, if he/she chooses to do so.
2.	An issue that needs to be closed, either due to not complying with this policy, or for other reasons, should be closed by a contributor.
3.	Issues that are accepted should be marked with appropriate labels.
4.	Issues that could impact functionality for many users should be considered severe. 
5.	Issues caused by the SDK or chip should not be marked severe, as there usually isn’t much to be done. Common sense should be applied when deciding. Such issues should be documented in the Wiki, for reference by users.
6.	Issues with feature requests should be discussed for viability/desirability.
7.	Feature requests or changes that are meant to address a very specific/limited use case, especially if at the expense of increased code complexity, may be denied, or may be required to be redesigned, generalized, or simplified.
8.	Feature requests that are not accompanied by a PR:
    * could be closed immediately (denied).
    * could be closed after some predetermined period of time (left as candidate for somebody to pick up).
9.	In some cases, feedback may be requested from the issue reporter, either as additional info for clarification, additional testing, or other. If no feedback is provided, the issue may be closed by a contributor or after 30 days by the STALE bot.

## Pull requests

A Pull Request (PR) is the process where code modifications are managed in GitHub.

The process is straight-forward.

 - Read [How to get faster PR reviews](https://github.com/kubernetes/community/blob/master/contributors/guide/pull-requests.md#best-practices-for-faster-reviews) by Kubernetes (but skip step 0)
 - Fork the Sonoff-Tasmota Repository [git repository](https://github.com/arendst/Sonoff-Tasmota).
 - Write/Change the code in your Fork for a new feature, bug fix, new sensor, optimization, etc.
 - Ensure tests work.
 - Create a Pull Request against the [**dev**](https://github.com/arendst/Sonoff-Tasmota/tree/dev) branch of Sonoff-Tasmota.

1. All pull requests must be done against the dev branch.
2. Only relevant files should be touched (Also beware if your editor has auto-formatting feature enabled).
3. Only one feature/fix should be added per PR.
4. If adding a new functionality (new hardware, new library support) not related to an existing component move it to it's own modules (.ino file).
5. PRs that don't compile (break Travis) or cause coding errors will not be merged. Please fix the issue. Same goes for PRs that are raised against older commit in dev - you might need to rebase and resolve conflicts.
6. All pull requests should undergo peer review by at least one contributor other than the creator, excepts for the owner.
7. All pull requests should consider updates to the documentation.
8. Pull requests that address an outstanding issue, particularly an issue deemed to be severe, should be given priority.
9. If a PR is accepted, then it should undergo review and updated based on the feedback provided, then merged.
10. Pull requests that don't meet the above will be denied and closed.

<Other>
<A table should be maintained for relating maintainers and components. When triaging, this is essential to figure out if someone in particular should be consulted about specific changes.>
<A stable release cadence should be established, e.g.: every month.>
