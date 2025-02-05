#
# eChronos Real-Time Operating System
# Copyright (C) 2015  National ICT Australia Limited (NICTA), ABN 62 102 206 173.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, version 3, provided that these additional
# terms apply under section 7:
#
#   No right, title or interest in or to any trade mark, service mark, logo or
#   trade name of of National ICT Australia Limited, ABN 62 102 206 173
#   ("NICTA") or its licensors is granted. Modified versions of the Program
#   must be plainly marked as such, and must not be distributed using
#   "eChronos" as a trade mark or product name, or misrepresented as being the
#   original Program.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# @TAG(NICTA_AGPL)
#

from pylib.utils import Git, get_top_dir
from pylib.components import _sort_typedefs, _sort_by_dependencies, _DependencyNode, _UnresolvableDependencyError
from pylib.tasks import _Review, _Task, _InvalidTaskStateError
from nose.tools import assert_raises
import itertools
import os
import tempfile
import subprocess
import unittest


def test_empty():
    """Test whether an empty test can be run at all given the test setup in x.py."""
    pass


def test_git_branch_hash():
    repo_dir = get_top_dir()

    try:
        revid, _ = _get_git_revision_hash_and_time(repo_dir)
    except subprocess.CalledProcessError:
        raise unittest.SkipTest('Test requires code to be managed in a local git repository')

    g = Git(local_repository=repo_dir)
    assert revid == g.branch_hash(revid)


def test_sort_typedefs():
    typedefs = ['typedef uint8_t foo;',
                'typedef foo bar;',
                'typedef bar baz;']
    expected = '\n'.join(typedefs)
    for x in itertools.permutations(typedefs):
        assert _sort_typedefs('\n'.join(x)) == expected


def test_full_stop_in_reviewer_name():
    with tempfile.TemporaryDirectory() as dir:
        round = 0
        author = 'john.doe'
        review_file_path = os.path.join(dir, 'review-{}.{}'.format(round, author))
        open(review_file_path, 'w').close()
        review = _Review(review_file_path)
        assert review.author == author
        assert review.round == round


def test_resolve_dependencies():
    N = _DependencyNode
    a = N(('a',), ('b', 'c'))
    b = N(('b',), ('c',))
    c = N(('c',), ())
    nodes = (a, b, c)
    output = list(_sort_by_dependencies(nodes))
    assert output == [c, b, a]


def test_resolve_unresolvable_dependencies():
    N = _DependencyNode
    a = N(('a',), ('b',))
    b = N(('b',), ('c',))
    nodes = (a, b)
    try:
        output = list(_sort_by_dependencies(nodes))
        assert False
    except _UnresolvableDependencyError:
        pass


def test_resolve_cyclic_dependencies():
    N = _DependencyNode
    a = N(('a',), ())
    b = N(('b',), ('a',))
    c = N(('c',), ('b', 'd'))
    d = N(('d',), ('c',))
    nodes = (a, b, c, d)
    try:
        output = list(_sort_by_dependencies(nodes))
        assert False
    except _UnresolvableDependencyError:
        pass
    output = list(_sort_by_dependencies(nodes, ignore_cyclic_dependencies=True))
    assert sorted(output) == sorted(nodes)


# Workaround for the following tests for the pre-integration check that don't use the Git module
class DummyGit:
    def __init__(self, task_name):
        self.branches = []
        self.remote_branches = frozenset(["archive/%s" % task_name])


# Helper for the pre-integration check tests
def task_dummy_create(task_name):
    return _Task(task_name, os.path.dirname(os.path.abspath(__file__)), DummyGit(task_name))


def test_task_accepted():
    # This task was accepted without any rework reviews
    task_dummy_create("eeZMmO-cpp-friendly-headers")._check_is_accepted()


def test_rework_is_accepted():
    # This task had a rework review that was later accepted by its review author
    task_dummy_create("ogb1UE-kochab-documentation-base")._check_is_accepted()


def test_rework_not_accepted():
    # This task was erroneously integrated with a rework review not later accepted by its review author
    assert_raises(_InvalidTaskStateError, task_dummy_create("g256JD-kochab-mutex-timeout")._check_is_accepted)


def test_not_enough_accepted():
    # This task was integrated after being accepted by only one reviewer, before we placed a hard minimum in the check
    assert_raises(_InvalidTaskStateError, task_dummy_create("65N0RS-fix-x-test-regression")._check_is_accepted)
