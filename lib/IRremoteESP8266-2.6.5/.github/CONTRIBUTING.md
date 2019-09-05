# Contributing to the IRremoteESP8266 library

:+1::tada: First off, thanks for taking the time to contribute! :tada::+1:

The following is a set of guidelines for contributing to the IRremoteESP8266 library, hosted on GitHub. These are guidelines, [not rules](http://imgur.com/mSHi8). Use your best judgment, and feel free to propose changes to this document in a pull request.

#### Table Of Contents

[Code of Conduct](#code-of-conduct)

[How Can I Contribute?](#how-can-i-contribute)
  * [Reporting Bugs](#reporting-bugs)
  * [Pull Requests](#pull-requests)

[Styleguides](#styleguides)
  * [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
  * [Git Commit Messages](#git-commit-messages)


## Code of Conduct

This project and everyone participating in it is governed by the principle of ["Be excellent to each other"](http://www.imdb.com/title/tt0096928/quotes). That's it. TL;DR: _Don't be a jerk._

## How Can I Contribute?

### Reporting Bugs

This section guides you through submitting a bug report for the library. Following these guidelines helps maintainers and the community understand your report :pencil:, reproduce the behavior :computer: :computer:, and find related reports :mag_right:.

Before creating bug reports, please check [this list](#before-submitting-a-bug-report) as you might find out that you don't need to create one. When you are creating a bug report, please [include as much detail as possible](#how-do-i-submit-a-good-bug-report). Fill out [the required template](issue_template.md), the information it asks for helps us resolve issues faster.

> **Note:** If you find a **Closed** issue that seems like it's the same thing that you're experiencing, open a new issue and include a link to the original issue in the body of your new one.

#### Before Submitting A Bug Report

* **Check the [Troubleshooting Guide](https://github.com/crankyoldgit/IRremoteESP8266/wiki/Troubleshooting-Guide).** You might be able to find the cause of the problem and fix it yourself. Most importantly, check if you can reproduce the problem in the latest version (a.k.a. 'master') of the library.
* **Perform a [cursory search](https://github.com/issues?q=+is%3Aissue+repo%3Acrankyoldgit/IRremoteESP8266)** to see if the problem is already reported. If it has **and the issue is still open**, add a comment to the existing issue instead of opening a new one.

#### How Do I Submit A (Good) Bug Report?

Bugs are tracked as [GitHub issues](https://guides.github.com/features/issues/). Create an issue and provide the following information by filling in [the template](issue_template.md).

Explain the problem and include any additional details to help maintainers reproduce the problem:

* **Use a clear and descriptive title** for the issue to identify the problem.
* **Describe the exact steps which reproduce the problem** in as much detail as possible.
* **Provide specific examples to demonstrate the steps**. Include links to files or GitHub projects, or copy/pasteable snippets, which you use in those examples. If you're providing snippets in the issue, use [Markdown code blocks](https://help.github.com/articles/markdown-basics/#multiple-lines).
* **Describe the behavior you observed after following the steps** and point out what exactly is the problem with that behavior.
* **Explain which behavior you expected to see instead and why.**
* **If the problem wasn't triggered by a specific action**, describe what you were doing before the problem happened and share more information using the guidelines below.

Provide more context by answering these questions:

* **Can you reproduce the problem in one of the code examples?**
* **Did the problem start happening recently** (e.g. after updating to a new version of Arduino or the library) or was this always a problem?
* If the problem started happening recently, **can you reproduce the problem in an older version of the library?** What's the most recent version in which the problem doesn't happen? You can download older versions of the library from [the releases page](https://github.com/crankyoldgit/IRremoteESP8266/releases).
* **Can you reliably reproduce the issue?** If not, provide details about how often the problem happens and under which conditions it normally happens.

Include details about your configuration, circuit and environment:

* **Which version of the library are you using?** You can get the exact version by inspecting the `library.json` file in the root directory of the library.
* **What board are you running this on?**

### Pull Requests

* Do not include issue numbers in the PR title
* Include as much data and comments as practicle.
* Follow the [C++ style guide](https://google.github.io/styleguide/cppguide.html).
* Please write or ensure Unit Tests cover the change you are making, if you can.
* End all files with a newline
* Avoid platform-dependent code.
* Use c98 types where possible for better portablity.
* In almost all cases, code & documentation should be peer-reviewed by at least one other contributor.
* The code should pass all the existing testing infrastructure in Travis. e.g. Unit tests, cpplint, and basic compilation.
* State if you have tested this under real conditions if you have, and what other tests you may have carried out.

### Git Commit Messages

* Limit the first line to 72 characters or less
* Reference issues and pull requests liberally after the first line
* Humour is always acceptable. Be liberal with it. ;-)
* While not required, a comprehensive description of all the changes in the PR is best.
