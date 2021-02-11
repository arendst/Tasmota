<img src="/tools/logo/TASMOTA_FullLogo_Vector.svg" alt="Logo" align="right" height="76"/>

# Contributing

**Any contribution helps our team and makes Tasmota better for the entire community!**

Everybody is welcome and invited to contribute to Tasmota Project by:

* Testing newly released features and reporting issues.
* Providing Pull Requests (Features, Proof of Concepts, Language files or Fixes)
* Contributing missing documentation for features and devices in our [documentation](https://tasmota.github.io/docs/Contributing)

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
 - Fork the Tasmota Repository [git repository](https://github.com/arendst/Tasmota).
 - Write/Change the code in your Fork for a new feature, bug fix, new sensor, optimization, etc.
 - Ensure tests work.
 - Create a Pull Request against the [**development**](https://github.com/arendst/Tasmota/tree/development) branch of Tasmota.

1. All pull requests must be done against the development branch.
2. Only relevant files should be touched (Also beware if your editor has auto-formatting feature enabled).
3. Only one feature/fix should be added per PR.
4. If adding a new functionality (new hardware, new library support) not related to an existing component move it to it's own modules (.ino file).
5. PRs that don't compile (fail in CI Tests) or cause coding errors will not be merged. Please fix the issue. Same goes for PRs that are raised against older commit in development - you might need to rebase and resolve conflicts.
6. All pull requests should undergo peer review by at least one contributor other than the creator, excepts for the owner.
7. All pull requests should consider updates to the documentation.
8. Pull requests that address an outstanding issue, particularly an issue deemed to be severe, should be given priority.
9. If a PR is accepted, then it should undergo review and updated based on the feedback provided, then merged.
10. By submitting a PR, it is needed to use the provided PR template and check all boxes, performing the required tasks and accepting the CLA.
11. Pull requests that don't meet the above will be denied and closed.

--------------------------------------

## Contributor License Agreement (CLA)

```
By making a contribution to this project, I certify that:

(a) The contribution was created in whole or in part by me and I
    have the right to submit it under the GPL-3.0 license; or

(b) The contribution is based upon previous work that, to the best
    of my knowledge, is covered under an appropriate open source
    license and I have the right under that license to submit that
    work with modifications, whether created in whole or in part
    by me, under the GPL-3.0 license; or

(c) The contribution was provided directly to me by some other
    person who certified (a), (b) or (c) and I have not modified
    it.

(d) I understand and agree that this project and the contribution
    are public and that a record of the contribution (including all
    personal information I submit with it) is maintained indefinitely
    and may be redistributed consistent with this project or the open
    source license(s) involved.
```

This Contributor License Agreement (CLA) was adopted on April 1st, 2019.

The text of this license is available under the [Creative Commons Attribution-ShareAlike 3.0 Unported License](http://creativecommons.org/licenses/by-sa/3.0/).  It is based on the Linux [Developer Certificate Of Origin](http://elinux.org/Developer_Certificate_Of_Origin), but is modified to explicitly use the GPL-3.0 license and not mention sign-off (due to GitHub.com keeps an historial, with your user name, of PRs' commits and all editions on PR's comments).

To accept the CLA it is required to put a x between [ ] on `[ ] I accept the CLA` in the PR template when submitting it. The [ ] is an opt-in box, so you have to manually accept it.

**Why a CLA ?**

_"A Contributor Licence Agreement (CLA) is strongly recommended when accepting third party contributions to an open development project, such as an open source software project. In order to redistribute contributions, it is necessary to ensure that the project has the necessary rights to do so. A Contributor Licence Agreement is a lightweight agreement, signed by the copyright holder, that grants the necessary rights for the contribution to be redistributed as part of the project."_ [OSS Watch](http://oss-watch.ac.uk/resources/cla)

A CLA is a legal document in which you state _you are entitled to contribute the code/documentation/translation to the project_ you’re contributing to and that _you are willing to have it used in distributions and derivative works_. This means that should there be any kind of legal issue in the future as to the origins and ownership of any particular piece of code, then that project has the necessary forms on file from the contributor(s) saying they were permitted to make this contribution.

CLA is a safety because it also ensures that once you have provided a contribution, you cannot try to withdraw permission for its use at a later date. People can therefore use that software, confident that they will not be asked to stop using pieces of the code at a later date.

A __license__ grants "outbound" rights to the user of project.

A __CLA__ enables a contributor to grant "inbound" rights to a project.





<Other>
<A table should be maintained for relating maintainers and components. When triaging, this is essential to figure out if someone in particular should be consulted about specific changes.>
<A stable release cadence should be established, e.g.: every month.>
