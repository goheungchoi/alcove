import os
import shutil

# TODO: Write a function that organize the file structure.
def clean_generator_dir(current_workspace_directory):
  cmake_folder:str = os.path.join(current_workspace_directory, 'cmake')
  config_folder:str = os.path.join(current_workspace_directory, 'config')
  script_folder:str = os.path.join(current_workspace_directory, 'scripts')

  cmake_files:list[str] = map(lambda filename: cmake_folder+'/'+filename, os.listdir(cmake_folder))
  config_files:list[str] = map(lambda filename: config_folder+'/'+filename, os.listdir(config_folder))
  script_files:list[str] = map(lambda filename: script_folder+'/'+filename, os.listdir(script_folder))

  files:list[str] = []
  files.extend(cmake_files)
  files.extend(config_files)
  files.extend(script_files)

  for file in files:
    print("FileOrganizer: Remove ", file)
    os.remove(file)

def organize_generator_dir(current_workspace_directory):
  # The `cmake` folder is set to be the `generators_folder` in Conan file.
  # Thus, this function will search the `cmake` folder and move files other than `.cmake` files. 
  cmake_folder:str = os.path.join(current_workspace_directory, 'cmake')
  config_folder:str = os.path.join(current_workspace_directory, 'config')
  script_folder:str = os.path.join(current_workspace_directory, 'scripts')

  generated_files:list[str] = os.listdir(cmake_folder)
  for file in generated_files:
    if file.endswith('.bat') or file.endswith('.sh'):
      print("-- Move ", file," from ", cmake_folder," to ", script_folder)
      shutil.move(os.path.join(cmake_folder, file), script_folder)
    elif file.endswith('.json'):
      print("-- Move ", file," from ", cmake_folder," to ", config_folder)
      shutil.move(os.path.join(cmake_folder, file), config_folder)
