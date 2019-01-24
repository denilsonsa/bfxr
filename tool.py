#!/usr/bin/env python3
import argparse
import urllib.request


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('url')
    parser.add_argument('target')
    args = parser.parse_args()
    urllib.request.urlretrieve(args.url, args.target)

if __name__ == "__main__":
    main()
