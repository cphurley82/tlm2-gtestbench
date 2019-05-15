# Downloads and extracts external dependencies needed to compile.
# If ext directory already exists, this will fail.
# On macOS, if you get a SSL: CERTIFICATE_VERIFY_FAILED error, try this: https://stackoverflow.com/a/45018725

import urllib.request
import zipfile
import pathlib
import os

dependencies = {'systemc': 'https://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.3.zip',
                'googletest': 'https://github.com/google/googletest/archive/release-1.8.1.zip',
                }

dependency_directory = 'ext'
print('Creating Dependency Directory: ' + dependency_directory)
pathlib.Path(dependency_directory).mkdir(exist_ok=False)

for dependency, file_url in dependencies.items():
    print('Downloading file: ' + file_url)
    zip_file_path = dependency_directory + '/' + dependency + '.zip'
    file_handle: object = urllib.request.urlretrieve(file_url, zip_file_path)

    zip_file_object = zipfile.ZipFile(zip_file_path, 'r')
    unzip_directory = dependency_directory + '/' + str(zip_file_object.namelist()[0]).strip('/')
    print('Extracting to: ' + unzip_directory)
    zip_file_object.extractall(path=dependency_directory)

    rename_directory = dependency_directory + '/' + dependency
    print('Renaming to: ' + rename_directory)
    os.rename(unzip_directory, rename_directory)
