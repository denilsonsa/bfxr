#!/usr/bin/env python3
import argparse
import urllib.request
import zipfile
import re
import os


def fake_sed(reg, rep, path):

    with open(path, "r") as sources:
        lines = sources.readlines()
    with open(path, "w") as sources:
        for line in lines:
            new_line = re.sub(reg, rep, line)
            sources.write(new_line)
            if line == new_line:
                print('same line: ', line)
            else:
                print('line was ', line)
                print('line is ', new_line)


def handle_gitfix(args):
    gitmodules = os.path.join(args.root, '.gitmodules')
    print('.gitmodules is', gitmodules)
    fake_sed(r'git@github.com', 'https:\\/\\/github.com\\/', gitmodules)


def handle_download(args):
    print('Downloading', args.url)
    print('Target', args.target)
    urllib.request.urlretrieve(args.url, args.target)
    print('Downloaded.')


def handle_unzip(args):
    print('Unzipping', args.zip)
    print('Target', args.target)
    zip = zipfile.ZipFile(args.zip, 'r')
    zip.extractall(args.target)
    zip.close()
    print('Extracted')


def main():
    parser = argparse.ArgumentParser()
    parser.set_defaults(handler=None)
    subparsers = parser.add_subparsers()

    gitfix_parser = subparsers.add_parser('gitfix')
    gitfix_parser.set_defaults(handler=handle_gitfix)
    gitfix_parser.add_argument('root')

    download_parser = subparsers.add_parser('download')
    download_parser.set_defaults(handler=handle_download)
    download_parser.add_argument('url')
    download_parser.add_argument('target')

    zip_parser = subparsers.add_parser('unzip')
    zip_parser.set_defaults(handler=handle_unzip)
    zip_parser.add_argument('zip')
    zip_parser.add_argument('target')

    args = parser.parse_args()
    if args.handler is None:
        parser.print_help()
    else:
        args.handler(args)

if __name__ == "__main__":
    main()
