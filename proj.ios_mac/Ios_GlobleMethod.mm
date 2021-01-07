//
//  Ios_GlobleMethod.mm
//  deer
//
//  Created by xinhua on 15/6/10.
//
//

#import "Ios_GlobleMethod.h"

std::string getDeviceID_IOS()
{
    NSString *id = [[NSUserDefaults standardUserDefaults] objectForKey:@"UUID"];    //获取标识为"UUID"的值
    if(id == nil)
    {
        if([[[UIDevice currentDevice] systemVersion] floatValue] > 6.0)
        {
            NSString *identifierNumber = [[NSUUID UUID] UUIDString];                //ios 6.0 之后可以使用的api
            [[NSUserDefaults standardUserDefaults] setObject:identifierNumber forKey:@"UUID"];  //保存为UUID
            [[NSUserDefaults standardUserDefaults] synchronize];
        }
        else{
            CFUUIDRef uuid = CFUUIDCreate(NULL);
            CFStringRef uuidString = CFUUIDCreateString(NULL, uuid);                    //ios6.0之前使用的api
            NSString *identifierNumber = [NSString stringWithFormat:@"%@", uuidString];
            [[NSUserDefaults standardUserDefaults] setObject:identifierNumber forKey:@"UUID"];
            [[NSUserDefaults standardUserDefaults] synchronize];
            CFRelease(uuidString);
            CFRelease(uuid);
        }
        id = [[NSUserDefaults standardUserDefaults] objectForKey:@"UUID"];
        return [id UTF8String];
    }
    return [id UTF8String];
}
