-- phpMyAdmin SQL Dump
-- version 3.4.11.1deb1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Feb 28, 2013 at 07:07 PM
-- Server version: 5.5.28
-- PHP Version: 5.4.4-11

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `gpio`
--

-- --------------------------------------------------------

--
-- Table structure for table `auto_start`
--
DROP TABLE IF EXISTS `auto_start`;
CREATE TABLE IF NOT EXISTS `auto_start` (
  `pinID` int(11) NOT NULL AUTO_INCREMENT,
  `pinNumber` varchar(2) COLLATE utf8_unicode_ci NOT NULL,
  `pinHours` varchar(2) COLLATE utf8_unicode_ci NOT NULL,
  `pinMins` varchar(2) COLLATE utf8_unicode_ci NOT NULL,
  `needUpdate` varchar(2) COLLATE utf8_unicode_ci NOT NULL,
  `autoDuration` varchar(3) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`pinID`),
  UNIQUE KEY `pinNumber` (`pinNumber`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=9 ;

--
-- Dumping data for table `auto_start`
--

INSERT INTO `auto_start` (`pinID`, `pinNumber`, `pinHours`, `pinMins`, `needUpdate`, `autoDuration`) VALUES
(1, '0', '23', '58', '0', '3'),
(2, '1', '15', '37', '0', '1'),
(3, '2', '0', '0', '0', '0'),
(4, '3', '0', '0', '0', '0'),
(5, '4', '0', '0', '0', '0'),
(6, '5', '0', '0', '0', '0'),
(7, '6', '0', '0', '0', '0'),
(8, '7', '0', '0', '0', '0');

-- --------------------------------------------------------

--
-- Table structure for table `doLoop`
--
DROP TABLE IF EXISTS `doLoop`;
CREATE TABLE IF NOT EXISTS `doLoop` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `numbers` varchar(2) COLLATE utf8_unicode_ci NOT NULL,
  `doLoop` varchar(2) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `numbers` (`numbers`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=9 ;

--
-- Dumping data for table `doLoop`
--

INSERT INTO `doLoop` (`ID`, `numbers`, `doLoop`) VALUES
(1, '1', '1');

-- --------------------------------------------------------

--
-- Table structure for table `garage`
--
DROP TABLE IF EXISTS `garage`;
CREATE TABLE IF NOT EXISTS `garage` (
  `pinID` int(11) NOT NULL AUTO_INCREMENT,
  `pinNumber` varchar(2) COLLATE utf8_unicode_ci NOT NULL,
  `isOpen` varchar(2) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`pinID`),
  UNIQUE KEY `pinNumber` (`pinNumber`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=9 ;

--
-- Dumping data for table `garage`
--

INSERT INTO `garage` (`pinID`, `pinNumber`, `isOpen`) VALUES
(1, '10', '1');

-- --------------------------------------------------------

--
-- Table structure for table `manual_start`
--
DROP TABLE IF EXISTS `manual_start`;
CREATE TABLE IF NOT EXISTS `manual_start` (
  `pinID` int(11) NOT NULL AUTO_INCREMENT,
  `pinNumber` varchar(2) COLLATE utf8_unicode_ci NOT NULL,
  `pinStart` varchar(2) COLLATE utf8_unicode_ci NOT NULL,
  `manualDuration` varchar(3) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`pinID`),
  UNIQUE KEY `pinNumber` (`pinNumber`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=9 ;

--
-- Dumping data for table `manual_start`
--

INSERT INTO `manual_start` (`pinID`, `pinNumber`, `pinStart`, `manualDuration`) VALUES
(1, '0', '0', '2'),
(2, '1', '0', '3'),
(3, '2', '0', '0'),
(4, '3', '0', '0'),
(5, '4', '0', '0'),
(6, '5', '0', '0'),
(7, '6', '0', '0'),
(8, '7', '0', '0');

-- --------------------------------------------------------

--
-- Table structure for table `pinAutoMode`
--
DROP TABLE IF EXISTS `pinAutoMode`;
CREATE TABLE IF NOT EXISTS `pinAutoMode` (
  `pinID` int(11) NOT NULL AUTO_INCREMENT,
  `pinNumber` varchar(2) COLLATE utf8_unicode_ci NOT NULL,
  `pinAutoMode` varchar(2) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`pinID`),
  UNIQUE KEY `pinNumber` (`pinNumber`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=9 ;

--
-- Dumping data for table `pinAutoMode`
--

INSERT INTO `pinAutoMode` (`pinID`, `pinNumber`, `pinAutoMode`) VALUES
(1, '0', '2'),
(2, '1', '0'),
(3, '2', '0'),
(4, '3', '0'),
(5, '4', '0'),
(6, '5', '0'),
(7, '6', '0'),
(8, '7', '0');

-- --------------------------------------------------------

--
-- Table structure for table `pinDays`
--
DROP TABLE IF EXISTS `pinDays`;
CREATE TABLE IF NOT EXISTS `pinDays` (
  `pinID` int(11) NOT NULL AUTO_INCREMENT,
  `pinNumber` varchar(2) COLLATE utf8_unicode_ci NOT NULL,
  `pinDays` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`pinID`),
  UNIQUE KEY `pinNumber` (`pinNumber`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=9 ;

--
-- Dumping data for table `pinDays`
--

INSERT INTO `pinDays` (`pinID`, `pinNumber`, `pinDays`) VALUES
(1, '0', 127),
(2, '1', 33),
(3, '2', 7),
(4, '3', 0),
(5, '4', 0),
(6, '5', 0),
(7, '6', 0),
(8, '7', 0);

-- --------------------------------------------------------

--
-- Table structure for table `pinDescription`
--
DROP TABLE IF EXISTS `pinDescription`;
CREATE TABLE IF NOT EXISTS `pinDescription` (
  `pinID` int(11) NOT NULL AUTO_INCREMENT,
  `pinNumber` varchar(2) COLLATE utf8_unicode_ci NOT NULL,
  `pinDescription` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`pinID`),
  UNIQUE KEY `pinNumber` (`pinNumber`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=9 ;

--
-- Dumping data for table `pinDescription`
--

INSERT INTO `pinDescription` (`pinID`, `pinNumber`, `pinDescription`) VALUES
(1, '0', 'Front Yard'),
(2, '1', 'Side Yards'),
(3, '2', 'Back of Back Yard'),
(4, '3', 'Back Yard by House'),
(5, '4', 'City Strips'),
(6, '5', 'Unused'),
(7, '6', 'Unused'),
(8, '7', 'Garage Door');

-- --------------------------------------------------------

--
-- Table structure for table `pinDirection`
--
DROP TABLE IF EXISTS `pinDirection`;
CREATE TABLE IF NOT EXISTS `pinDirection` (
  `pinID` int(11) NOT NULL AUTO_INCREMENT,
  `pinNumber` varchar(2) COLLATE utf8_unicode_ci NOT NULL,
  `pinDirection` varchar(2) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`pinID`),
  UNIQUE KEY `pinNumber` (`pinNumber`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=9 ;

--
-- Dumping data for table `pinDirection`
--

INSERT INTO `pinDirection` (`pinID`, `pinNumber`, `pinDirection`) VALUES
(1, '0', '0'),
(2, '1', '0'),
(3, '2', '0'),
(4, '3', '0'),
(5, '4', '0'),
(6, '5', '0'),
(7, '6', '0'),
(8, '7', '0');

-- --------------------------------------------------------

--
-- Table structure for table `pinMode`
--
DROP TABLE IF EXISTS `pinMode`;
CREATE TABLE IF NOT EXISTS `pinMode` (
  `pinID` int(11) NOT NULL AUTO_INCREMENT,
  `pinNumber` varchar(2) COLLATE utf8_unicode_ci NOT NULL,
  `pinMode` varchar(1) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`pinID`),
  UNIQUE KEY `pinNumber` (`pinNumber`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=9 ;

--
-- Dumping data for table `pinMode`
--

INSERT INTO `pinMode` (`pinID`, `pinNumber`, `pinMode`) VALUES
(1, '0', '0'),
(2, '1', '1'),
(3, '2', '0'),
(4, '3', '0'),
(5, '4', '0'),
(6, '5', '0'),
(7, '6', '0'),
(8, '7', '0');

-- --------------------------------------------------------

--
-- Table structure for table `pinStatus`
--
DROP TABLE IF EXISTS `pinStatus`;
CREATE TABLE IF NOT EXISTS `pinStatus` (
  `pinID` int(11) NOT NULL AUTO_INCREMENT,
  `pinNumber` varchar(2) COLLATE utf8_unicode_ci NOT NULL,
  `pinStatus` varchar(1) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`pinID`),
  UNIQUE KEY `pinNumber` (`pinNumber`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=9 ;

--
-- Dumping data for table `pinStatus`
--

INSERT INTO `pinStatus` (`pinID`, `pinNumber`, `pinStatus`) VALUES
(1, '0', '0'),
(2, '1', '0'),
(3, '2', '0'),
(4, '3', '0'),
(5, '4', '0'),
(6, '5', '0'),
(7, '6', '0'),
(8, '7', '0');

-- --------------------------------------------------------

--
-- Table structure for table `Season`
--
DROP TABLE IF EXISTS `Season`;
CREATE TABLE IF NOT EXISTS `Season` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `adjust` varchar(4) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `adjust` (`adjust`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=2 ;

--
-- Dumping data for table `Season`
--

INSERT INTO `Season` (`ID`, `adjust`) VALUES
(1, '-5');

-- --------------------------------------------------------

--
-- Table structure for table `Temperature`
--
DROP TABLE IF EXISTS `Temperature`;
CREATE TABLE IF NOT EXISTS `Temperature` (
  `tempID` int(11) NOT NULL AUTO_INCREMENT,
  `minTemp` varchar(4) COLLATE utf8_unicode_ci NOT NULL,
  `currTemp` varchar(4) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`tempID`),
  UNIQUE KEY `minTemp` (`minTemp`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=2 ;

--
-- Dumping data for table `Temperature`
--

INSERT INTO `Temperature` (`tempID`, `minTemp`, `currTemp`) VALUES
(1, '-3', '0');

-- --------------------------------------------------------

--
-- Table structure for table `users`
--
DROP TABLE IF EXISTS `users`;
CREATE TABLE IF NOT EXISTS `users` (
  `userID` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(28) COLLATE utf8_unicode_ci NOT NULL,
  `password` varchar(64) COLLATE utf8_unicode_ci NOT NULL,
  `salt` varchar(8) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`userID`),
  UNIQUE KEY `username` (`username`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=2 ;

--
-- Dumping data for table `users`
--

INSERT INTO `users` (`userID`, `username`, `password`, `salt`) VALUES
(1, 'admin', '547c88bc221b7626a605764aae6dd802a63f6ad8df03a41941ac5b03898b7f4f', 'a2360f6a');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
