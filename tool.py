#!/usr/bin/env python3
import argparse
import urllib.request
import zipfile

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
