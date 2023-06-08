#!/usr/bin/env python3

from edid import Edid
from dataclasses import dataclass
import sys
import os
import re

DEFAULT_DIRECTORY = '/sys/class/drm'

connectorPattern = re.compile("card(\\d+)-(.+)-(\\d+)")
allCaps = re.compile("[a-zA-Z]{3}")

def nameAndValOrNone(name, val):
    if val is None:
        return ""
    return "{}: {}\n".format(name, str(val))

def noneOrCompare(listed, given):
    if given is not None:
        if isinstance(listed, str):
            if given.lower() != listed.lower():
                return False
        elif given != listed:
            return False
    return True

@dataclass(frozen=True)
class DisplayInfo:
    card : int | None
    connectorType : str | None
    connector : int | None
    headerChecksum : int | None
    calcChecksum : int | None
    manufacturerID : str | None
    productCode : int | None
    serial : int | None
    prodWeek : int | None
    prodYear : int | None
    modelYear : int | None
    name : str | None
    strSerial : str | None

    def __str__(self):
        return "{}{}{}{}{}{}{}{}{}{}{}{}{}".format(
            nameAndValOrNone("Card", self.card),
            nameAndValOrNone("Connector Type", self.connectorType),
            nameAndValOrNone("Connector", self.connector),
            nameAndValOrNone("EDID Checksum", self.headerChecksum),
            nameAndValOrNone("Calculated Checksum", self.calcChecksum),
            nameAndValOrNone("Manufacturer ID", self.manufacturerID),
            nameAndValOrNone("Product Code", self.productCode),
            nameAndValOrNone("Numeric Serial", self.serial),
            nameAndValOrNone("Production Week", self.prodWeek),
            nameAndValOrNone("Production Year", self.prodYear),
            nameAndValOrNone("Model Year", self.modelYear),
            nameAndValOrNone("Name", self.name),
            nameAndValOrNone("Serial", self.strSerial))

    def match(self, given):
        return (noneOrCompare(self.connectorType, given.connectorType) and
                noneOrCompare(self.headerChecksum, given.headerChecksum) and
                noneOrCompare(self.calcChecksum, given.calcChecksum) and
                noneOrCompare(self.manufacturerID, given.manufacturerID) and
                noneOrCompare(self.productCode, given.productCode) and
                noneOrCompare(self.serial, given.serial) and
                noneOrCompare(self.prodWeek, given.prodWeek) and
                noneOrCompare(self.prodYear, given.prodYear) and
                noneOrCompare(self.modelYear, given.modelYear) and
                noneOrCompare(self.name, given.name) and
                noneOrCompare(self.strSerial, given.strSerial))

def get_DisplayInfo(card, connectorType, connector, edid):
    if connectorType == 'DP':
        connectorType = 'DisplayPort'

    calcChecksum = 0
    for byte in edid.data:
        calcChecksum += byte
    calcChecksum &= 0xFF
    calcChecksum = 256 - calcChecksum

    manu_id = ""
    for letter in edid.header.manu_id.letter:
        manu_id = "{}{}".format(manu_id, letter.name.upper())
    prodYear = None
    if edid.header.year:
        prodYear = edid.header.year
    modelYear = None
    if edid.header.model_year:
        modelYear = edid.header.model_year

    name = ""
    strSerial = ""
    for desc in enumerate(edid.descriptor_blocks.descriptor_block):
        if not desc[1].is_dtd:
            num = desc[0]
            if desc[1].desc.type == Edid.DescType.display_name:
                desc = desc[1].desc.display_name
                name = desc.text.decode('cp437')
            elif desc[1].desc.type == Edid.DescType.display_serial:
                desc = desc[1].desc.display_serial
                strSerial = desc.text.decode('cp437')

    return DisplayInfo(card,
                       connectorType,
                       connector - 1,
                       edid.checksum,
                       calcChecksum,
                       manu_id,
                       edid.header.product_code,
                       edid.header.serial,
                       edid.header.week,
                       prodYear,
                       modelYear,
                       name.strip(),
                       strSerial.strip())

def get_DisplayInfos(directory):
    displayInfos = []

    for item in os.scandir("/sys/class/drm"):
        if item.is_dir():
            match = connectorPattern.fullmatch(item.name)
            if match is not None:
                with open("{}/status".format(item.path), 'r') as status:
                    if status.readline().strip() != 'connected':
                        continue
                displayInfos.append(get_DisplayInfo(int(match.group(1)),
                                                    match.group(2),
                                                    int(match.group(3)),
                                                    Edid.from_file("{}/edid".format(item.path))))

    return displayInfos

def strOrNone(value):
    if len(value) == 0:
        return None
    return value

def intOrNone(name, value):
    if len(value) == 0:
        return None
    try:
        return int(value)
    except ValueError:
        raise ValueError("Item {} requires an integer.".format(name))

def assemble_DisplayInfo(args):
    card = None
    connectorType = None
    connector = None
    headerChecksum = None
    calcChecksum = None
    manufacturerID = None
    productCode = None
    serial = None
    prodWeek = None
    prodYear = None
    modelYear = None
    name = None
    strSerial = None

    for item in args:
        key, value = item.split('=')
        match key.translate({' ': None, '_': None}).lower():
            case 'card':
                card = intOrNone(key, value)
            case 'connectortype':
                connectorType = strOrNone(value)
            case 'connector':
                connector = intOrNone(key, value)
            case 'headerchecksum':
                headerChecksum = intOrNone(key, value)
            case 'calcchecksum' | 'calculatedchecksum':
                calcChecksum = intOrNone(key, value)
            case 'manufacturerid':
                if allCaps.match(value) is None:
                    raise ValueError("Manufacturer ID must be 3 letters.")
                manufacturerID = strOrNone(value.upper())
            case 'productcode':
                productCode = intOrNone(key, value)
            case 'numericserial':
                serial = intOrNone(key, value)
            case 'prodweek' | 'productionweek':
                prodWeek = intOrNone(key, value)
            case 'prodyear' | 'productionyear':
                prodYear = intOrNone(key, value)
            case 'modelyear':
                modelYear = intOrNone(key, value)
            case 'name':
                name = strOrNone(value)
            case 'serial':
                strSerial = strOrNone(value)
            case other:
                raise ValueError("Unknown key {}.".format(other))

    return DisplayInfo(card,
                       connectorType,
                       connector,
                       headerChecksum,
                       calcChecksum,
                       manufacturerID,
                       productCode,
                       serial,
                       prodWeek,
                       prodYear,
                       modelYear,
                       name,
                       strSerial)

if __name__ == '__main__':
    displayInfos = get_DisplayInfos(DEFAULT_DIRECTORY)
    if len(sys.argv) < 2:
        for num, info in enumerate(displayInfos):
            print("Display: {}".format(num))
            print(info)
    else:
        matchInfo = assemble_DisplayInfo(sys.argv[1:])

        found = None
        for info in displayInfos:
            if info.match(matchInfo):
                if found is not None:
                    raise ValueError("Multiple displays match!")
                found = info

        if found is not None:
            print("{}-{}".format(found.connectorType, found.connector), end='')
        else:
            exit(1)
