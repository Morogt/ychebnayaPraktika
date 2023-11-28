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
-- Temporary view structure for view `group_composition_view`
--

DROP TABLE IF EXISTS `group_composition_view`;
/*!50001 DROP VIEW IF EXISTS `group_composition_view`*/;
SET @saved_cs_client     = @@character_set_client;
/*!50503 SET character_set_client = utf8mb4 */;
/*!50001 CREATE VIEW `group_composition_view` AS SELECT 
 1 AS `id`,
 1 AS `sale_date`,
 1 AS `tourist_full_name`,
 1 AS `tour_group_name`,
 1 AS `ticket_price`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `sales_ledger_view`
--

DROP TABLE IF EXISTS `sales_ledger_view`;
/*!50001 DROP VIEW IF EXISTS `sales_ledger_view`*/;
SET @saved_cs_client     = @@character_set_client;
/*!50503 SET character_set_client = utf8mb4 */;
/*!50001 CREATE VIEW `sales_ledger_view` AS SELECT 
 1 AS `id`,
 1 AS `date`,
 1 AS `tour_group_name`,
 1 AS `hotel_name`,
 1 AS `total_cost`*/;
SET character_set_client = @saved_cs_client;

--
-- Final view structure for view `group_composition_view`
--

/*!50001 DROP VIEW IF EXISTS `group_composition_view`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8mb4 */;
/*!50001 SET character_set_results     = utf8mb4 */;
/*!50001 SET collation_connection      = utf8mb4_0900_ai_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`user`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `group_composition_view` AS select `gc`.`id` AS `id`,`gc`.`sale_date` AS `sale_date`,concat(`t`.`name`,' ',`t`.`surname`,' ',`t`.`lastname`) AS `tourist_full_name`,`tg`.`Groupname` AS `tour_group_name`,`gc`.`ticket_price` AS `ticket_price` from ((`group_composition` `gc` join `tourists` `t` on((`gc`.`tourist_id` = `t`.`id`))) join `tour_groups` `tg` on((`gc`.`group_id` = `tg`.`id`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `sales_ledger_view`
--

/*!50001 DROP VIEW IF EXISTS `sales_ledger_view`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8mb4 */;
/*!50001 SET character_set_results     = utf8mb4 */;
/*!50001 SET collation_connection      = utf8mb4_0900_ai_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`user`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `sales_ledger_view` AS select `sl`.`id` AS `id`,`sl`.`date` AS `date`,`tg`.`Groupname` AS `tour_group_name`,`h`.`hotel_name` AS `hotel_name`,`sl`.`total_cost` AS `total_cost` from ((`sales_ledger` `sl` join `tour_groups` `tg` on((`sl`.`tour_group_id` = `tg`.`id`))) join `hotels` `h` on((`sl`.`hotel_id` = `h`.`id`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2023-11-28 20:59:28
