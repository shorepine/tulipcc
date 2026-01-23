{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs =
    {
      nixpkgs,
      flake-utils,
      ...
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        # Import nixpkgs for the specific system
        pkgs = import nixpkgs { inherit system; };

        littlefs-python =
          {
            buildPythonPackage,
            cython,
            setuptools,
            setuptools-scm,
            ...
          }:
          buildPythonPackage rec {
            pname = "littlefs-python";
            version = "0.17.0";

            src = pkgs.fetchFromGitHub {
              owner = "jrast";
              repo = pname;
              tag = "v${version}";
              fetchSubmodules = true;
              hash = "sha256-GRMYnnt8E1QNWoynDJyXOIzE1AtzeXOwBv/ZrIDWSGs=";
            };

            buildInputs = [
              cython
              setuptools-scm
            ];

            pyproject = true;
            build-system = [ setuptools ];
          };

        python = pkgs.python3.override {
          self = python;
          packageOverrides = pyfinal: pyprev: {
            littlefs-python = pyfinal.callPackage littlefs-python { };
          };
        };
      in
      {
        devShells.default = pkgs.mkShell {
          name = "tulipcc-devshell";

          packages = with pkgs; [
            esptool
            cmake
            ninja
            dfu-util

            (python.withPackages (python-pkgs: [
              python-pkgs.littlefs-python
              python-pkgs.cython
            ]))
          ];
        };
      }
    );
}
