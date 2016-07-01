import re

class Comment:
    def __init__(self):
        self.comment = []
        self.name = ""

    def __str__(self):
        result = "Name:\n"  + self.name + "\n"
        result += "Comment:\n"
	for line in self.comment:
            result += line
        return result


def trim(string):
    return string.strip(' \n\r\t')


def ltrim(string):
	return string.lstrip(' \t')


def is_single_line_comment(line):
    single_line_comments = ["//", "///", "//!"]
    line = trim(line)
    for prefix in single_line_comments:
        if line.startswith(prefix):
            return True
    return False

def is_multi_line_comment(line):
    multi_line_comments_start = "/*"
    line = trim(line)
    if line.startswith(multi_line_comments_start):
        return True

    return False


def is_comment(line):
    return is_single_line_comment(line) or is_multi_line_comment(line)

def is_template(line):
    line = trim(line)
    return line.startswith("template ") or line.startswith("template<");


def skip_empty_lines(file,line):
    while( trim(line) == "" ):
        line = file.readline()
    return line


def read_template_definition(file, line):
    if not is_template(line):
        return ""

    opening_angles = line.count('<')
    closing_angles = line.count('>')
    name = line
    while opening_angles > closing_angles:
        line = file.readline()
        name += line
        opening_angles += line.count('<')
        closing_angles += line.count('>')
    return name


def read_name(file, line):
    if is_template(line):
        name = read_template_definition(file, line)
        line = file.readline()
    else:
        name = ""

    opening_brackets = line.count('(')
    closing_brackets = line.count(')')
    name += line;
    while opening_brackets > closing_brackets:
        line = file.readline()
        opening_brackets += line.count('(')
        closing_brackets += line.count(')')
        name += line
        file.readline()

    name = trim(name)
    if name.endswith(";\n"):
        name = name[0:len(name)-2] + "\n"
    return trim(name)


def read_comment(file,line,comment):
    while is_comment(line):
        line = ltrim(line)
        comment.append(line)
        line = file.readline()

    return line


def read_comment_and_name(file, line):
    comment = Comment()
    line = read_comment(file, line, comment.comment)
    line = skip_empty_lines(file, line)
    comment.name = read_name(file, line)
    return comment


def extract_comments(filename):
    comments = []
    file = open(filename, 'r+')
    while True:
        line = file.readline()
        if not line: break
        if is_comment(line):
            comment = read_comment_and_name(file, line)
            comments.append(comment)
    file.close()

#    for comment in comments:
#        print(comment)

    return comments


def extract_includes(filename):
    includes = []
    file = open(filename, 'r+')
    while True:
        line = file.readline()
        if not line: break
        line = ltrim(line)
        if line.startswith('#include '):
            includes.append(line)
        

    file.close()

    return includes


def extract_include_guard(filename):
    # detect classical include guard
    archetypes = open(filename).read()
    guard_regex = re.compile(r'#ifndef\s+([^\s]+)[^\n]*\n#define\s+\1')
    match = guard_regex.search(archetypes)
    if match and match.start() == archetypes.index('#'):
        return '''#ifndef {0}
#define {0}

'''.format(match.group(1))

    # detect pragma once include guard
    guard_regex = re.compile(r'#pragma\s+once\s+')
    match = guard_regex.search(archetypes)
    if match and match.start() == archetypes.index('#'):
        return '#pragma once\n\n'
    
    return None


if __name__ == "__main__":
    extract_comments("test.hh")
