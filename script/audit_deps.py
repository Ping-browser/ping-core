#!/usr/bin/env python

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/.

import sys
import json
import argparse
import subprocess


def main():
    args = parse_args()
    audit_deps(args)


def audit_deps(args):
    npm_args = ['npm', 'audit']

    # Just run audit regularly if --audit_dev_deps is passed
    if args.audit_dev_deps:
        subprocess.call(npm_args)
        return

    npm_args.append('--json')
    audit_process = subprocess.Popen(npm_args, stdout=subprocess.PIPE)
    output = audit_process.communicate()[0]

    try:
        result = json.loads(str(output))
        resolutions = result['actions'][0]['resolves']
        non_dev_exceptions = [r for r in resolutions if not r['dev']]
    except ValueError:
        # This can happen in the case of an NPM network error
        print('audit failed to return valid json')
        return

    # Trigger a failure if there are non-dev exceptions
    if non_dev_exceptions:
        raise Exception(output)

    # Still pass if there are dev exceptions, but let the user know about them
    if resolutions:
        print('Audit finished, there are dev package warnings')
    else:
        print('Audit finished, no vulnerabilities found')

    print(output)


def parse_args():
    parser = argparse.ArgumentParser(description='Audit brave-core npm deps')
    parser.add_argument('--audit_dev_deps',
                        action='store_true',
                        help='Audit dev dependencies')
    return parser.parse_args()


if __name__ == '__main__':
    sys.exit(main())
