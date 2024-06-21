# Clean previous builds
python setup.py clean --all
rm -rf build/ dist/ mypackage.egg-info

# Build the extensions in place
python setup.py build_ext --inplace

# Install the package (optional)
pip install .
