# AWS IoT Topic Rule – Water Tank Alert

## 📌 Rule Query

```sql
SELECT * FROM 'esp32/tank/data' WHERE water_level < 25
