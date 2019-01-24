#!/usr/bin/env python3
import argparse
import urllib.request


def handle_download(args):
    print('Downloading', args.url)
    print('Target', args.target)
    urllib.request.urlretrieve(args.url, args.target)
    print('Downloaded.')


def main():
    parser = argparse.ArgumentParser()
    parser.set_defaults(handler=None)
    subparsers = parser.add_subparsers()

    download_parser = subparsers.add_parser('download')
    download_parser.set_defaults(handler=handle_download)
    download_parser.add_argument('url')
    download_parser.add_argument('target')

    args = parser.parse_args()
    if args.handler is None:
        parser.print_help()
    else:
        args.handler(args)

if __name__ == "__main__":
    main()
