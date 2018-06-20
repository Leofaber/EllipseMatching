function res = elltest(C1,D1,R1,M1,C2,D2,R2,M2,tol)
%ELLTEST Summary of this function goes here
%   Detailed explanation goes here

if nargin < 9
    tol = 1e-12;
end

fprintf('elltest()\n');

disp('D1');
disp(D1);
disp('R1');
disp(R1)


% First transformation:
% E1 became a unit circle centered on the origin
C3 = sqrt(D1)*R1'*(C2-C1);


g=sprintf('%.32f\n ', C3);
fprintf('C3:\n %s\n', g)

S1 = [1/sqrt(D1(1,1)) 0 ; 0 1/sqrt(D1(2,2))];

printf('S1:\n');
disp(S1);
printf('R1:\n');
disp(R1');
printf('M2:\n');
disp(M2);

M3 = S1*R1'*M2*R1*S1;

[rows, columns] = size(M3);
for row=1:rows
  for col=1:columns
    if M3(row,col) <= tol
      printf('Element %d,%d of matrix M3 is approximated to 0\n',row-1,col-1);
      M3(row,col) = 0;
    end
  end
end


printf('M3:\n');
disp(M3);


% Second trnsformation:
% Rotate to align E2 with the x/y axis
[R4,D4] = eig(M3);

printf('Matrice autovettori:\n');
disp(R4);
disp(D4);


C4 = R4'*C3;

printf('C4:\n');
disp(C4);


%g=sprintf('%.32f\n ', C4);
%fprintf('C4:\n %s\n', g)

% Quartic equation for extremal points
d1 = D4(1,1);
d2 = D4(2,2);
c1 = C4(1);
c2 = C4(2);

printf('d1,d2,c1,c2:\n %.32f\n %.32f\n %.32f\n %.32f\n',d1,d2,c1,c2);


if c1 == 0 && c2 == 0 % Extremal points are along x/y axes at distance sqrt(1/d1) and sqrt(1/d2)

    if d1 == d2
        ds = sqrt(1/d1);
    else
        ds = [sqrt(1/d1) sqrt(1/d2)];
    end

else % Extremal points shall be compute usign a quartic

    % Quartic eq. a*s^4 + b*s^3 + c*s^2 + d*s + e = 0
    a = d1^2*d2^2;
    b = -2*d1^2*d2 - 2*d1*d2^2;
    c = d1^2 + d2^2 +4*d1*d2 - d1*d2^2*c1^2 - d1^2*d2*c2^2;
    d = -2*d2 - 2*d1 + 2*d1*d2*c1^2 + 2*d1*d2*c2^2;
    e = 1 - d1*c1^2 - d2*c2^2;

    % Get roots
    s = quartic([a b c d e], 1e-6);


    if isreal(s)
        g=sprintf('%.32f\n ', s);
        fprintf('s from quartic:\n %s\n', g)
    end

    disp('s from quartic:');
    disp(s);

    % Get real non-zero roots
    id = [];
    for i = 1 : length(s)
        if isreal(s(i))
            if s(i) ~= 0
                id = [id i];
            end
        end
    end
    s = s(id);

    disp('real non-zero roots:  ');
    disp(s);








    % Get extremal points

    px_num = d1*c1*s;

    %g=sprintf('%.32f\n ', px_num);
    %fprintf('px_num:\n %s\n', g)

    % Approximation to 0
    iii = 1;
    for px_num_elem = px_num
        if abs(px_num_elem) <= tol
          %  fprintf('%.32f -> reduced to 0\n', px_num_elem )
            px_num(iii) = 0;
        end
        iii++;
    end
    %g=sprintf('%.32f\n ', px_num);
    %fprintf('px_num:\n %s\n', g)

    px_den = (d1*s - 1);

  %  g=sprintf('%.32f\n ', px_den);
  %  fprintf('px_den:\n %s\n', g)

    % Approximation to 0
    iii = 1;
    for px_den_elem = px_den
        if abs(px_den_elem) <= tol
            %fprintf('%.32f -> reduced to 0\n', px_den_elem )
            px_den(iii) = 0;
        end
        iii++;
    end
  %  g=sprintf('%.32f\n ', px_den);
  %  fprintf('px_den:\n %s\n', g)



    px = px_num ./ px_den;

    if isreal(px)
      g=sprintf('%.32f\n ', px);
      fprintf('px:\n %s\n', g)
    else
      disp('px:')
      disp(px)
    end
















    py_num = d2*c2*s;

  %  g=sprintf('%.32f\n ', py_num);
  %  fprintf('py_num:\n %s\n', g)

    % Approximation to 0
    iii = 1;
    for py_num_elem = py_num
        if abs(py_num_elem) <= tol
            %fprintf('%.32f -> reduced to 0\n', py_num_elem )
            py_num(iii) = 0;
        end
        iii++;
    end
  %  g=sprintf('%.32f\n ', py_num);
  %  fprintf('py_num:\n %s\n', g)


    py_den = (d2*s - 1);
%    g=sprintf('%.32f\n ', py_den);
  %  fprintf('py_den:\n %s\n', g)

    % Approximation to 0
    iii = 1;
    for py_den_elem = py_den
        if abs(py_den_elem) <= tol
            %fprintf('%.32f -> reduced to 0\n', py_den_elem )
            py_den(iii) = 0;
        end
        iii++;
    end
  %  g=sprintf('%.32f\n ', py_den);
  %  fprintf('py_den:\n %s\n', g)






    py = py_num ./ py_den;

    if isreal(py)
      g=sprintf('%.32f\n ', py);
      fprintf('py:\n %s\n', g);
    else
      disp('py:');
      disp(py);
    end






  %  px = d1*c1*s./(d1*s - 1);
  %  py = d2*c2*s./(d2*s - 1);

    % Elimination of NaN values from px
    iii = 1;
    for px_elem = px
        if isnan(px_elem)
            px(iii) = 0;
        end
        iii++;
    end
    % Elimination of NaN values from py
    iii = 1;
    for py_elem = py
        if isnan(py_elem)
            py(iii) = 0;
        end
        iii++;
    end



    ds_temp = sqrt(px.^2 + py.^2);

  %  g=sprintf('%.32f\n ', ds_temp);
%    fprintf('ds_temp:\n %s\n', g)


    ds = [];

    % Elimination of NaN and Inf values
    iii = 1;
    for ps_elem = ds_temp
        if isnan(ps_elem) || isinf(ps_elem)

        else
          ds = [ds ps_elem]
        end
        iii++;
    end



end


if isreal(ds)
  g=sprintf('%.32f\n ', ds);
  fprintf('DS:\n %s\n', g)
else
  disp('ds:');
  disp(ds);
end









% Intersection query

% Error
res = -1;

% Check numeric precision
MAX_DS = max(ds);
MIN_DS = min(ds);


fprintf('DS_MAX: %.32f\n', max(ds));
if abs(1-max(ds)) <= tol
  disp('---> max(ds) reduced to 1')
  MAX_DS = 1;
end

fprintf('DS_MIN: %.32f\n', min(ds));
if abs(1-min(ds)) <= tol
  disp('---> min(ds) reduced to 1')
  MIN_DS = 1;
end


if MAX_DS < 1

    % fprintf('E2 is contained in E1\n');
    res = 0;

elseif MAX_DS > 1

    if MIN_DS < 1

        % fprintf('E2 overlap E1\n');
        res = 3;

    elseif MIN_DS > 1

        % Check whether the circle centre is inside or not the ellipse
        f0 = d1*c1^2 + d2*c2^2 - 1;
        fprintf('f0 ---> %.32f\n', f0);
        if abs(f0) < tol
          f0 = 0;
          fprintf('-->f0 Reduced to 0\n');
        end

        if f0 > 0

            % fprintf('E2 is external to E1\n')
            res = 7;

        else

            % fprintf('E2 contains E1\n')
            res = 5;

        end

    else

        % Check whether the circle centre is inside or not the ellipse
        f0 = d1*c1^2 + d2*c2^2 - 1;
        fprintf('f0 ---> %.32f\n', f0);
        if abs(f0) < tol
          f0 = 0;
          fprintf('-->f0 Reduced to 0\n');
        end

        if f0 > 0

            % fprintf('E2 is external to E1 but tangent\n');
            res = 6;

        else

            % fprintf('E2 contains E1 but tangent\n');
            res = 4;

        end

    end

else

    if MIN_DS < 1

        % fprintf('E2 is contained in E1 but tangent\n');
        res = 1;

    else

        % fprintf('E2 is equal to E1\n');
        res = 2;

    end

end

end
