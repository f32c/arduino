#!/bin/sh

# BM_URL_BASE="http://localhost/~guest/fpgarduino/bm"
#LOCALDIR="."
#BM_URL_BASE="http://www.nxlab.fer.hr/fpgarduino/bm"
#LOCALDIR="bm"
BM_URL_BASE="https://raw.githubusercontent.com/f32c/fpgarduino/master"
LOCALDIR="."

FPGARDUINO_NAME="FPGArduino"
FPGARDUINO_VERSION="1.0.0"
FPGARDUINO_ALL="fpgarduino.zip"

BOOTLOADERS_NAME="bootloaders"
BOOTLOADERS_VERSION="1.0.0"
BOOTLOADERS_ALL="bootloaders.zip"

UJPROG_NAME="ujprog"
UJPROG_VERSION="1.0.0"
UJPROG_WIN32="ujprog-win32.zip"
UJPROG_APPLE64="ujprog-x86_64-apple-darwin.tar.bz2"
UJPROG_LINUX32="ujprog-i686-pc-linux-gnu.tar.bz2"
UJPROG_LINUX64="ujprog-x86_64-pc-linux-gnu.tar.bz2"

XC3SPROG_NAME="xc3sprog"
XC3SPROG_VERSION="795"
XC3SPROG_WIN32="xc3sprog-771-win32.zip"
XC3SPROG_APPLE64="xc3sprog-771-win32.zip"
XC3SPROG_LINUX32="xc3sprog-774-linux32.tar.bz2"
XC3SPROG_LINUX64="xc3sprog-795-linux64.tar.bz2"

OPENOCD_NAME="openocd"
OPENOCD_VERSION="0.9.0"
OPENOCD_WIN32="openocd-0.9.0-windows.zip"
OPENOCD_APPLE64="openocd-0.9.0-windows.zip"
OPENOCD_LINUX32="openocd-0.9.0-linux32.tar.bz2"
OPENOCD_LINUX64="openocd-0.9.0-linux64.tar.bz2"

FLEAFPGA_NAME="FleaFPGA-JTAG"
FLEAFPGA_VERSION="12.2"
FLEAFPGA_WIN32="fleafpga-jtag-12.2-win32.zip"
FLEAFPGA_APPLE64="fleafpga-jtag-12.2-linux32.tar.bz2"
FLEAFPGA_LINUX32="fleafpga-jtag-12.2-linux32.tar.bz2"
FLEAFPGA_LINUX64="fleafpga-jtag-12.2-linux32.tar.bz2"

NUMATO_NAME="numato"
NUMATO_VERSION="1.0.0"
NUMATO_WIN32="numato-1.0.0-python3.zip"
NUMATO_APPLE64="numato-1.0.0-python3.zip"
NUMATO_LINUX32="numato-1.0.0-python3.zip"
NUMATO_LINUX64="numato-1.0.0-python3.zip"

POSEDGE1PROG_NAME="posedge1prog"
POSEDGE1PROG_VERSION="1.0.0"
POSEDGE1PROG_WIN32="posedge1prog-100.zip"
POSEDGE1PROG_APPLE64="posedge1prog-100.zip"
POSEDGE1PROG_LINUX32="posedge1prog-100.zip"
POSEDGE1PROG_LINUX64="posedge1prog-100.zip"

F32CUP_NAME="f32cup"
F32CUP_VERSION="1.0.0"
F32CUP_WIN32="f32cup-1.0.0-python3.zip"
F32CUP_APPLE64="f32cup-1.0.0-python3.zip"
F32CUP_LINUX32="f32cup-1.0.0-python3.zip"
F32CUP_LINUX64="f32cup-1.0.0-python3.zip"

COMPILER_NAME="f32c-compiler"
COMPILER_VERSION="1.0.0"
COMPILER_WIN32="f32c-win32-toolchain-7_2.zip"
COMPILER_APPLE64="f32c-osx-toolchain-7_2.tar.gz"
COMPILER_LINUX32="f32c-lin32-toolchain-7_2.tar.gz"
COMPILER_LINUX64="f32c-lin64-toolchain-7_2.tar.gz"

SHA256()
{
  sha256sum ${LOCALDIR}/$1 | cut -f 1 -d " "
}

SIZE()
{
  wc -c ${LOCALDIR}/$1 | cut -f 1 -d " "
}

# FPGARDUINO_ZIP_SIZE=$(wc $FPGARDUINO_ZIP | sed -e "s/ *\([0-9]\+\) *\([0-9]\+\) *\([0-9]\+\).*/\\3/g")
cat << EOF
{
  "packages": 
  [
    {
      "name":"${FPGARDUINO_NAME}",
      "maintainer":"EMARD",
      "websiteURL":"http://www.nxlab.fer.hr/fpgarduino",
      "email":"vordah@gmail.com",
      "help":
      {
        "online":"http://www.nxlab.fer.hr/fpgarduino"
      },
      "platforms":
      [ 
        {
          "name":"${FPGARDUINO_NAME}",
          "architecture":"f32c",
          "version":"${FPGARDUINO_VERSION}",
          "category":"FPGA",
          "url":"${BM_URL_BASE}/${FPGARDUINO_ALL}",
          "archiveFileName":"${FPGARDUINO_ALL}",
          "checksum":"SHA-256:$(SHA256 ${FPGARDUINO_ALL})",
          "size":"$(SIZE ${FPGARDUINO_ALL})",
          "help":
          {
            "online":"http://www.nxlab.fer.hr/fpgarduino"
          },
          "boards":
          [ 
            {
              "name":"FPGA"
            }
          ],
          "toolsDependencies":
          [
            {
              "packager":"${FPGARDUINO_NAME}",
              "name":"${XC3SPROG_NAME}",
              "version":"${XC3SPROG_VERSION}"
            },
            {
              "packager":"${FPGARDUINO_NAME}",
              "name":"${OPENOCD_NAME}",
              "version":"${OPENOCD_VERSION}"
            },
            {
              "packager":"${FPGARDUINO_NAME}",
              "name":"${FLEAFPGA_NAME}",
              "version":"${FLEAFPGA_VERSION}"
            },
            {
              "packager":"${FPGARDUINO_NAME}",
              "name":"${NUMATO_NAME}",
              "version":"${NUMATO_VERSION}"
            },
            {
              "packager":"${FPGARDUINO_NAME}",
              "name":"${POSEDGE1PROG_NAME}",
              "version":"${POSEDGE1PROG_VERSION}"
            },
            {
              "packager":"${FPGARDUINO_NAME}",
              "name":"${F32CUP_NAME}",
              "version":"${F32CUP_VERSION}"
            },
            {
              "packager":"${FPGARDUINO_NAME}",
              "name":"${BOOTLOADERS_NAME}",
              "version":"${BOOTLOADERS_VERSION}"
            },
            {
              "packager":"${FPGARDUINO_NAME}",
              "name":"${UJPROG_NAME}",
              "version":"${UJPROG_VERSION}"
            },
            {
              "packager":"${FPGARDUINO_NAME}",
              "name":"${COMPILER_NAME}",
              "version":"${COMPILER_VERSION}"
            }
          ]
        } 
      ],
      "tools": 
      [
        {
          "name":"${XC3SPROG_NAME}",
          "version":"${XC3SPROG_VERSION}",
          "systems":
          [
            {
              "host":"i686-mingw32",
              "url":"${BM_URL_BASE}/${XC3SPROG_WIN32}",
              "archiveFileName":"${XC3SPROG_WIN32}",
              "checksum":"SHA-256:$(SHA256 ${XC3SPROG_WIN32})",
              "size":"$(SIZE ${XC3SPROG_WIN32})"
            },
            {
              "host":"x86_64-apple-darwin",
              "url":"${BM_URL_BASE}/${XC3SPROG_APPLE64}",
              "archiveFileName":"${XC3SPROG_APPLE64}",
              "checksum":"SHA-256:$(SHA256 ${XC3SPROG_APPLE64})",
              "size":"$(SIZE ${XC3SPROG_APPLE64})"
            },
            {
              "host":"i686-pc-linux-gnu",
              "url":"${BM_URL_BASE}/${XC3SPROG_LINUX32}",
              "archiveFileName":"${XC3SPROG_LINUX32}",
              "checksum":"SHA-256:$(SHA256 ${XC3SPROG_LINUX32})",
              "size":"$(SIZE ${XC3SPROG_LINUX32})"
            },
            {
              "host":"x86_64-pc-linux-gnu",
              "url":"${BM_URL_BASE}/${XC3SPROG_LINUX64}",
              "archiveFileName":"${XC3SPROG_LINUX64}",
              "checksum":"SHA-256:$(SHA256 ${XC3SPROG_LINUX64})",
              "size":"$(SIZE ${XC3SPROG_LINUX64})"
            }
          ]
        },
        {
          "name":"${OPENOCD_NAME}",
          "version":"${OPENOCD_VERSION}",
          "systems":
          [
            {
              "host":"i686-mingw32",
              "url":"${BM_URL_BASE}/${OPENOCD_WIN32}",
              "archiveFileName":"${OPENOCD_WIN32}",
              "checksum":"SHA-256:$(SHA256 ${OPENOCD_WIN32})",
              "size":"$(SIZE ${OPENOCD_WIN32})"
            },
            {
              "host":"x86_64-apple-darwin",
              "url":"${BM_URL_BASE}/${OPENOCD_APPLE64}",
              "archiveFileName":"${OPENOCD_APPLE64}",
              "checksum":"SHA-256:$(SHA256 ${OPENOCD_APPLE64})",
              "size":"$(SIZE ${OPENOCD_APPLE64})"
            },
            {
              "host":"i686-pc-linux-gnu",
              "url":"${BM_URL_BASE}/${OPENOCD_LINUX32}",
              "archiveFileName":"${OPENOCD_LINUX32}",
              "checksum":"SHA-256:$(SHA256 ${OPENOCD_LINUX32})",
              "size":"$(SIZE ${OPENOCD_LINUX32})"
            },
            {
              "host":"x86_64-pc-linux-gnu",
              "url":"${BM_URL_BASE}/${OPENOCD_LINUX64}",
              "archiveFileName":"${OPENOCD_LINUX64}",
              "checksum":"SHA-256:$(SHA256 ${OPENOCD_LINUX64})",
              "size":"$(SIZE ${OPENOCD_LINUX64})"
            }
          ]
        },
        {
          "name":"${FLEAFPGA_NAME}",
          "version":"${FLEAFPGA_VERSION}",
          "systems":
          [
            {
              "host":"i686-mingw32",
              "url":"${BM_URL_BASE}/${FLEAFPGA_WIN32}",
              "archiveFileName":"${FLEAFPGA_WIN32}",
              "checksum":"SHA-256:$(SHA256 ${FLEAFPGA_WIN32})",
              "size":"$(SIZE ${FLEAFPGA_WIN32})"
            },
            {
              "host":"x86_64-apple-darwin",
              "url":"${BM_URL_BASE}/${FLEAFPGA_APPLE64}",
              "archiveFileName":"${FLEAFPGA_APPLE64}",
              "checksum":"SHA-256:$(SHA256 ${FLEAFPGA_APPLE64})",
              "size":"$(SIZE ${FLEAFPGA_APPLE64})"
            },
            {
              "host":"i686-pc-linux-gnu",
              "url":"${BM_URL_BASE}/${FLEAFPGA_LINUX32}",
              "archiveFileName":"${FLEAFPGA_LINUX32}",
              "checksum":"SHA-256:$(SHA256 ${FLEAFPGA_LINUX32})",
              "size":"$(SIZE ${FLEAFPGA_LINUX32})"
            },
            {
              "host":"x86_64-pc-linux-gnu",
              "url":"${BM_URL_BASE}/${FLEAFPGA_LINUX64}",
              "archiveFileName":"${FLEAFPGA_LINUX64}",
              "checksum":"SHA-256:$(SHA256 ${FLEAFPGA_LINUX64})",
              "size":"$(SIZE ${FLEAFPGA_LINUX64})"
            }
          ]
        },
        {
          "name":"${NUMATO_NAME}",
          "version":"${NUMATO_VERSION}",
          "systems":
          [
            {
              "host":"i686-mingw32",
              "url":"${BM_URL_BASE}/${NUMATO_WIN32}",
              "archiveFileName":"${NUMATO_WIN32}",
              "checksum":"SHA-256:$(SHA256 ${NUMATO_WIN32})",
              "size":"$(SIZE ${NUMATO_WIN32})"
            },
            {
              "host":"x86_64-apple-darwin",
              "url":"${BM_URL_BASE}/${NUMATO_APPLE64}",
              "archiveFileName":"${NUMATO_APPLE64}",
              "checksum":"SHA-256:$(SHA256 ${NUMATO_APPLE64})",
              "size":"$(SIZE ${NUMATO_APPLE64})"
            },
            {
              "host":"i686-pc-linux-gnu",
              "url":"${BM_URL_BASE}/${NUMATO_LINUX32}",
              "archiveFileName":"${NUMATO_LINUX32}",
              "checksum":"SHA-256:$(SHA256 ${NUMATO_LINUX32})",
              "size":"$(SIZE ${NUMATO_LINUX32})"
            },
            {
              "host":"x86_64-pc-linux-gnu",
              "url":"${BM_URL_BASE}/${NUMATO_LINUX64}",
              "archiveFileName":"${NUMATO_LINUX64}",
              "checksum":"SHA-256:$(SHA256 ${NUMATO_LINUX64})",
              "size":"$(SIZE ${NUMATO_LINUX64})"
            }
          ]
        },
        {
          "name":"${POSEDGE1PROG_NAME}",
          "version":"${POSEDGE1PROG_VERSION}",
          "systems":
          [
            {
              "host":"i686-mingw32",
              "url":"${BM_URL_BASE}/${POSEDGE1PROG_WIN32}",
              "archiveFileName":"${POSEDGE1PROG_WIN32}",
              "checksum":"SHA-256:$(SHA256 ${POSEDGE1PROG_WIN32})",
              "size":"$(SIZE ${POSEDGE1PROG_WIN32})"
            },
            {
              "host":"x86_64-apple-darwin",
              "url":"${BM_URL_BASE}/${POSEDGE1PROG_APPLE64}",
              "archiveFileName":"${POSEDGE1PROG_APPLE64}",
              "checksum":"SHA-256:$(SHA256 ${POSEDGE1PROG_APPLE64})",
              "size":"$(SIZE ${POSEDGE1PROG_APPLE64})"
            },
            {
              "host":"i686-pc-linux-gnu",
              "url":"${BM_URL_BASE}/${POSEDGE1PROG_LINUX32}",
              "archiveFileName":"${POSEDGE1PROG_LINUX32}",
              "checksum":"SHA-256:$(SHA256 ${POSEDGE1PROG_LINUX32})",
              "size":"$(SIZE ${POSEDGE1PROG_LINUX32})"
            },
            {
              "host":"x86_64-pc-linux-gnu",
              "url":"${BM_URL_BASE}/${POSEDGE1PROG_LINUX64}",
              "archiveFileName":"${POSEDGE1PROG_LINUX64}",
              "checksum":"SHA-256:$(SHA256 ${POSEDGE1PROG_LINUX64})",
              "size":"$(SIZE ${POSEDGE1PROG_LINUX64})"
            }
          ]
        },
        {
          "name":"${F32CUP_NAME}",
          "version":"${F32CUP_VERSION}",
          "systems":
          [
            {
              "host":"i686-mingw32",
              "url":"${BM_URL_BASE}/${F32CUP_WIN32}",
              "archiveFileName":"${F32CUP_WIN32}",
              "checksum":"SHA-256:$(SHA256 ${F32CUP_WIN32})",
              "size":"$(SIZE ${F32CUP_WIN32})"
            },
            {
              "host":"x86_64-apple-darwin",
              "url":"${BM_URL_BASE}/${F32CUP_APPLE64}",
              "archiveFileName":"${F32CUP_APPLE64}",
              "checksum":"SHA-256:$(SHA256 ${F32CUP_APPLE64})",
              "size":"$(SIZE ${F32CUP_APPLE64})"
            },
            {
              "host":"i686-pc-linux-gnu",
              "url":"${BM_URL_BASE}/${F32CUP_LINUX32}",
              "archiveFileName":"${F32CUP_LINUX32}",
              "checksum":"SHA-256:$(SHA256 ${F32CUP_LINUX32})",
              "size":"$(SIZE ${F32CUP_LINUX32})"
            },
            {
              "host":"x86_64-pc-linux-gnu",
              "url":"${BM_URL_BASE}/${F32CUP_LINUX64}",
              "archiveFileName":"${F32CUP_LINUX64}",
              "checksum":"SHA-256:$(SHA256 ${F32CUP_LINUX64})",
              "size":"$(SIZE ${F32CUP_LINUX64})"
            }
          ]
        },
        {
          "name":"${BOOTLOADERS_NAME}",
          "version":"${BOOTLOADERS_VERSION}",
          "systems":
          [
            {
              "host":"i686-mingw32",
              "url":"${BM_URL_BASE}/${BOOTLOADERS_ALL}",
              "archiveFileName":"${BOOTLOADERS_ALL}",
              "checksum":"SHA-256:$(SHA256 ${BOOTLOADERS_ALL})",
              "size":"$(SIZE ${BOOTLOADERS_ALL})"
            },
            {
              "host":"x86_64-apple-darwin",
              "url":"${BM_URL_BASE}/${BOOTLOADERS_ALL}",
              "archiveFileName":"${BOOTLOADERS_ALL}",
              "checksum":"SHA-256:$(SHA256 ${BOOTLOADERS_ALL})",
              "size":"$(SIZE ${BOOTLOADERS_ALL})"
            },
            {
              "host":"i686-pc-linux-gnu",
              "url":"${BM_URL_BASE}/${BOOTLOADERS_ALL}",
              "archiveFileName":"${BOOTLOADERS_ALL}",
              "checksum":"SHA-256:$(SHA256 ${BOOTLOADERS_ALL})",
              "size":"$(SIZE ${BOOTLOADERS_ALL})"
            },
            {
              "host":"x86_64-pc-linux-gnu",
              "url":"${BM_URL_BASE}/${BOOTLOADERS_ALL}",
              "archiveFileName":"${BOOTLOADERS_ALL}",
              "checksum":"SHA-256:$(SHA256 ${BOOTLOADERS_ALL})",
              "size":"$(SIZE ${BOOTLOADERS_ALL})"
            }
          ]
        },
        {
          "name":"${UJPROG_NAME}",
          "version":"${UJPROG_VERSION}",
          "systems":
          [
            {
              "host":"i686-mingw32",
              "url":"${BM_URL_BASE}/${UJPROG_WIN32}",
              "archiveFileName":"${UJPROG_WIN32}",
              "checksum":"SHA-256:$(SHA256 ${UJPROG_WIN32})",
              "size":"$(SIZE ${UJPROG_WIN32})"
            },
            {
              "host":"x86_64-apple-darwin",
              "url":"${BM_URL_BASE}/${UJPROG_APPLE64}",
              "archiveFileName":"${UJPROG_APPLE64}",
              "checksum":"SHA-256:$(SHA256 ${UJPROG_APPLE64})",
              "size":"$(SIZE ${UJPROG_APPLE64})"
            },
            {
              "host":"i686-pc-linux-gnu",
              "url":"${BM_URL_BASE}/${UJPROG_LINUX32}",
              "archiveFileName":"${UJPROG_LINUX32}",
              "checksum":"SHA-256:$(SHA256 ${UJPROG_LINUX32})",
              "size":"$(SIZE ${UJPROG_LINUX32})"
            },
            {
              "host":"x86_64-pc-linux-gnu",
              "url":"${BM_URL_BASE}/${UJPROG_LINUX64}",
              "archiveFileName":"${UJPROG_LINUX64}",
              "checksum":"SHA-256:$(SHA256 ${UJPROG_LINUX64})",
              "size":"$(SIZE ${UJPROG_LINUX64})"
            }
          ]
        },
        {
          "name":"${COMPILER_NAME}",
          "version":"${COMPILER_VERSION}",
          "systems":
          [
            {
              "host":"i686-mingw32",
              "url":"${BM_URL_BASE}/${COMPILER_WIN32}",
              "archiveFileName":"${COMPILER_WIN32}",
              "checksum":"SHA-256:$(SHA256 ${COMPILER_WIN32})",
              "size":"$(SIZE ${COMPILER_WIN32})"
            },
            {
              "host":"x86_64-apple-darwin",
              "url":"${BM_URL_BASE}/${COMPILER_APPLE64}",
              "archiveFileName":"${COMPILER_APPLE64}",
              "checksum":"SHA-256:$(SHA256 ${COMPILER_APPLE64})",
              "size":"$(SIZE ${COMPILER_APPLE64})"
            },
            {
              "host":"i686-pc-linux-gnu",
              "url":"${BM_URL_BASE}/${COMPILER_LINUX32}",
              "archiveFileName":"${COMPILER_LINUX32}",
              "checksum":"SHA-256:$(SHA256 ${COMPILER_LINUX32})",
              "size":"$(SIZE ${COMPILER_LINUX32})"
            },
            {
              "host":"x86_64-pc-linux-gnu",
              "url":"${BM_URL_BASE}/${COMPILER_LINUX64}",
              "archiveFileName":"${COMPILER_LINUX64}",
              "checksum":"SHA-256:$(SHA256 ${COMPILER_LINUX64})",
              "size":"$(SIZE ${COMPILER_LINUX64})"
            }
          ]
        }      
      ]
    } 
  ]
}
EOF
