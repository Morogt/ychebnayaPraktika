-- MySQL dump 10.13  Distrib 8.0.32, for Linux (x86_64)
--
-- Host: localhost    Database: tours
-- ------------------------------------------------------
-- Server version	8.0.35-0ubuntu0.22.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `group_composition`
--

DROP TABLE IF EXISTS `group_composition`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `group_composition` (
  `id` int NOT NULL AUTO_INCREMENT,
  `sale_date` date DEFAULT NULL,
  `tourist_id` int DEFAULT NULL,
  `group_id` int DEFAULT NULL,
  `ticket_price` decimal(10,2) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `tourist_id` (`tourist_id`),
  KEY `group_id` (`group_id`),
  CONSTRAINT `group_composition_ibfk_1` FOREIGN KEY (`tourist_id`) REFERENCES `tourists` (`id`),
  CONSTRAINT `group_composition_ibfk_2` FOREIGN KEY (`group_id`) REFERENCES `tour_groups` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `group_composition`
--

LOCK TABLES `group_composition` WRITE;
/*!40000 ALTER TABLE `group_composition` DISABLE KEYS */;
INSERT INTO `group_composition` VALUES (1,'2023-11-26',8,1,1444.00),(2,'2023-11-26',5,8,123.00),(3,'2023-11-26',8,5,12466.00),(4,'2023-11-26',1,1,125533.00),(7,'2023-11-26',3,2,2444.00),(8,'2023-11-26',8,11,13000.00);
/*!40000 ALTER TABLE `group_composition` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2023-11-28 20:59:28
