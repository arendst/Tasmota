# based on http://protips.readthedocs.io/link-roles.html

from docutils import nodes
from repo_util import run_cmd_get_output

def get_github_rev():
    path = run_cmd_get_output('git rev-parse --short HEAD')
    tag = run_cmd_get_output('git describe --exact-match')
    print 'Git commit ID: ', path
    if len(tag):
        print 'Git tag: ', tag
        path = tag
    return path


def setup(app):
    baseurl = 'https://github.com/krzychb/esp-epaper-29-ws'
    rev = get_github_rev()
    app.add_role('idf', autolink('{}/tree/{}/%s'.format(baseurl, rev)))
    app.add_role('idf_file', autolink('{}/blob/{}/%s'.format(baseurl, rev)))
    app.add_role('idf_raw', autolink('{}/raw/{}/%s'.format(baseurl, rev)))
    app.add_role('component', autolink('{}/tree/{}/components/%s'.format(baseurl, rev)))
    app.add_role('component_file', autolink('{}/blob/{}/components/%s'.format(baseurl, rev)))
    app.add_role('component_raw', autolink('{}/raw/{}/components/%s'.format(baseurl, rev)))
    app.add_role('example', autolink('{}/tree/{}/examples/%s'.format(baseurl, rev)))
    app.add_role('example_file', autolink('{}/blob/{}/examples/%s'.format(baseurl, rev)))
    app.add_role('example_raw', autolink('{}/raw/{}/examples/%s'.format(baseurl, rev)))

def autolink(pattern):
    def role(name, rawtext, text, lineno, inliner, options={}, content=[]):
        url = pattern % (text,)
        node = nodes.reference(rawtext, text, refuri=url, **options)
        return [node], []
    return role
