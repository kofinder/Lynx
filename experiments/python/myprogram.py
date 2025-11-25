from datetime import datetime, timedelta

# Get current date and time
now = datetime.now()
print("Now:", now)

# Formatting datetime to string
formatted = now.strftime("%Y-%m-%d %H:%M:%S")
print("Formatted:", formatted)

# Parsing string to datetime
date_string = "2025-07-04 12:30:00"
parsed_date = datetime.strptime(date_string, "%Y-%m-%d %H:%M:%S")
print("Parsed:", parsed_date)

# Adding 7 days
future = now + timedelta(days=7)
print("7 days later:", future)

# Subtracting 3 hours
past = now - timedelta(hours=3)
print("3 hours ago:", past)

# Difference between two dates
diff = future - past
print("Difference:", diff)
