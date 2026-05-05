///The spawner has two parts, one that spawns entities in the main thread and
///one that spawns entities in the graphics thread. Both spawners read from the
///same file, so entities are spawned in pairs. An entity in the main thread will
///know how to send a message to an entity in the graphics thread and vice-versa.
