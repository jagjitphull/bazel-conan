import cv2
import numpy as np
import math

def create_synthetic_lane_video():
    """
    Creates a synthetic video with lane markings for ADAS testing
    """
    # Video parameters
    width, height = 640, 480
    fps = 30
    duration = 10  # seconds
    total_frames = fps * duration
    
    # Create video writer
    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    out = cv2.VideoWriter('synthetic_test_video.mp4', fourcc, fps, (width, height))
    
    # Lane parameters
    lane_width = 200
    center_x = width // 2
    
    for frame_num in range(total_frames):
        # Create black background
        frame = np.zeros((height, width, 3), dtype=np.uint8)
        
        # Add road texture (gray)
        frame[:, :] = (50, 50, 50)
        
        # Add some noise for realism
        noise = np.random.normal(0, 10, (height, width, 3))
        frame = np.clip(frame.astype(np.float32) + noise, 0, 255).astype(np.uint8)
        
        # Simulate vehicle movement (lanes moving down)
        offset = (frame_num * 5) % 100
        
        # Draw lane markings
        for y in range(0, height + 100, 40):
            y_pos = y - offset
            if y_pos > height:
                continue
                
            # Left lane marking (solid white line)
            left_x = center_x - lane_width // 2
            cv2.line(frame, (left_x, max(0, y_pos)), 
                    (left_x, min(height, y_pos + 20)), (255, 255, 255), 3)
            
            # Right lane marking (solid white line)
            right_x = center_x + lane_width // 2
            cv2.line(frame, (right_x, max(0, y_pos)), 
                    (right_x, min(height, y_pos + 20)), (255, 255, 255), 3)
            
            # Center dashed line (yellow)
            if (y // 40) % 2 == 0:  # Dashed pattern
                cv2.line(frame, (center_x, max(0, y_pos)), 
                        (center_x, min(height, y_pos + 15)), (0, 255, 255), 2)
        
        # Add some curve to make it more interesting
        if frame_num > total_frames // 3:
            curve_offset = int(30 * math.sin(frame_num * 0.05))
            
            # Shift the lanes slightly
            M = np.float32([[1, 0, curve_offset], [0, 1, 0]])
            frame = cv2.warpAffine(frame, M, (width, height))
        
        # Add some shadows/lighting variation
        if frame_num > total_frames * 2 // 3:
            # Simulate changing lighting
            brightness = 1.0 + 0.3 * math.sin(frame_num * 0.1)
            frame = np.clip(frame * brightness, 0, 255).astype(np.uint8)
        
        # Write frame to video
        out.write(frame)
        
        # Show progress
        if frame_num % (total_frames // 10) == 0:
            print(f"Progress: {frame_num/total_frames*100:.0f}%")
    
    out.release()
    print("Synthetic test video created: synthetic_test_video.mp4")

def create_lane_change_scenario():
    """
    Creates a video specifically for testing lane change scenarios
    """
    width, height = 640, 480
    fps = 30
    duration = 15
    total_frames = fps * duration
    
    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    out = cv2.VideoWriter('lane_change_scenario.mp4', fourcc, fps, (width, height))
    
    for frame_num in range(total_frames):
        frame = np.zeros((height, width, 3), dtype=np.uint8)
        frame[:, :] = (40, 40, 40)  # Dark gray road
        
        # Three lanes scenario
        lane1_x = width // 4
        lane2_x = width // 2
        lane3_x = 3 * width // 4
        
        offset = (frame_num * 8) % 80
        
        # Draw lane markings
        for y in range(0, height + 80, 40):
            y_pos = y - offset
            if y_pos > height:
                continue
            
            # Lane boundaries
            cv2.line(frame, (lane1_x, max(0, y_pos)), 
                    (lane1_x, min(height, y_pos + 20)), (255, 255, 255), 3)
            cv2.line(frame, (lane3_x, max(0, y_pos)), 
                    (lane3_x, min(height, y_pos + 20)), (255, 255, 255), 3)
            
            # Center dashed lines
            if (y // 40) % 2 == 0:
                cv2.line(frame, (lane2_x, max(0, y_pos)), 
                        (lane2_x, min(height, y_pos + 15)), (255, 255, 0), 2)
        
        # Simulate vehicle starting in middle lane, then changing
        phase = frame_num / total_frames
        
        if phase < 0.3:
            # Start in middle lane
            vehicle_x = lane2_x
        elif phase < 0.7:
            # Gradual lane change to right
            progress = (phase - 0.3) / 0.4
            vehicle_x = int(lane2_x + (lane3_x - lane2_x) * progress)
        else:
            # Stabilize in right lane
            vehicle_x = lane3_x
        
        # Draw vehicle position indicator
        cv2.circle(frame, (vehicle_x, height - 50), 8, (0, 0, 255), -1)
        
        # Add some environmental elements
        if frame_num % 60 < 30:  # Blinking effect for realism
            cv2.circle(frame, (50, 50), 10, (0, 255, 0), -1)  # Traffic light
        
        out.write(frame)
        
        if frame_num % (total_frames // 10) == 0:
            print(f"Lane change scenario: {frame_num/total_frames*100:.0f}%")
    
    out.release()
    print("Lane change scenario created: lane_change_scenario.mp4")

if __name__ == "__main__":
    print("Creating synthetic test videos for ADAS...")
    create_synthetic_lane_video()
    create_lane_change_scenario()
    print("Done! Use 'synthetic_test_video.mp4' or 'lane_change_scenario.mp4' with your ADAS system.")